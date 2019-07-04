//来跟一下open函数
//open函数分配binder_proc对象，并初始化
static int binder_open(struct inode *nodp, struct file *filp)
    struct binder_proc *proc;
    struct binder_device *binder_dev;
    //BINDER_DEBUG_OPEN_CLOSE表示属于哪一类log，全局变量binder_debug_mask控制打开哪一类
    //默认打开USER_ERROR, FAILED_TRANSACTION, DEAD_TRANSACTION
    binder_debug(BINDER_DEBUG_OPEN_CLOSE, "%s: %d:%d\n", __func__,
             current->group_leader->pid, current->pid);
    //内核中current宏，可以打出当前进程的pid tid
         
    //分配并初始化binder_proc
    proc = kzalloc(sizeof(*proc), GFP_KERNEL);
    if (proc == NULL)
        return -ENOMEM;
    spin_lock_init(&proc->inner_lock);
    spin_lock_init(&proc->outer_lock);
    get_task_struct(current->group_leader);//task的引用计数+1
    proc->tsk = current->group_leader;//保存进程task
    mutex_init(&proc->files_lock);
    INIT_LIST_HEAD(&proc->todo);
    if (binder_supported_policy(current->policy)) 
        proc->default_priority.sched_policy = current->policy;
        proc->default_priority.prio = current->normal_prio;
    else 
        proc->default_priority.sched_policy = SCHED_NORMAL;
        proc->default_priority.prio = NICE_TO_PRIO(0);
         
    //获取binder_device
    binder_dev = container_of(filp->private_data, struct binder_device,
                  miscdev);
    proc->context = &binder_dev->context;
    //初始化alloc结构体成员，每个binder的binder分配器的进程状态
    binder_alloc_init(&proc->alloc);
        //pid赋值 初始化mutex list_head
        alloc->pid = current->group_leader->pid;
	    mutex_init(&alloc->mutex);
	    INIT_LIST_HEAD(&alloc->buffers);

    //已创建的一种binder类型++
    binder_stats_created(BINDER_STAT_PROC);
    proc->pid = current->group_leader->pid;
    INIT_LIST_HEAD(&proc->delivered_death);
    INIT_LIST_HEAD(&proc->waiting_threads);
    //将proc与文件描述符绑定，这样应用层操作文件的时候和proc能对应起来
    filp->private_data = proc;
    //将这个proc插入到全局哈希链表binder_procs头部     
    mutex_lock(&binder_procs_lock);
    hlist_add_head(&proc->proc_node, &binder_procs);
    mutex_unlock(&binder_procs_lock);
    //创建sysfs debug文件     
    if (binder_debugfs_dir_entry_proc) 
        char strbuf[11];
         
        snprintf(strbuf, sizeof(strbuf), "%u", proc->pid);
        proc->debugfs_entry = debugfs_create_file(strbuf, 0444,
            binder_debugfs_dir_entry_proc,
            (void *)(unsigned long)proc->pid,
            &binder_proc_fops);
    return 0;


//接下来看mmap函数,总结起来就是分配一块内核空间，与用户空间映射
static int binder_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	struct binder_proc *proc = filp->private_data;
	const char *failure_string;

	if (proc->tsk != current->group_leader)
		return -EINVAL;
    //最大分配4M内存
	if ((vma->vm_end - vma->vm_start) > SZ_4M)
		vma->vm_end = vma->vm_start + SZ_4M;

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "%s: %d %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     __func__, proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));
    //如果有mmap禁止的flag，当前FORBIDDEN_MMAM_FLAGS==VM_WRITE，即禁止写
	if (vma->vm_flags & FORBIDDEN_MMAP_FLAGS) 
		ret = -EPERM;
		failure_string = "bad vm_flags";
		goto err_bad_arg;
	vma->vm_flags |= VM_DONTCOPY | VM_MIXEDMAP;
	vma->vm_flags &= ~VM_MAYWRITE;

	vma->vm_ops = &binder_vm_ops;
	vma->vm_private_data = proc;
    //真正处理map的地方
	ret = binder_alloc_mmap_handler(&proc->alloc, vma);
	    struct binder_buffer *buffer;
        //int binder_alloc_mmap_handler(struct binder_alloc *alloc,struct vm_area_struct *vma)
        mutex_lock(&binder_alloc_mmap_lock);//一个全局的mutex锁
        if (alloc->buffer) //已被map过，则报错
            ret = -EBUSY;
            failure_string = "already mapped";
            goto err_already_mapped;
        //申请一段内存
        area = get_vm_area(vma->vm_end - vma->vm_start, VM_ALLOC);
        if (area == NULL) 
            ret = -ENOMEM;
            failure_string = "get_vm_area";
            goto err_get_vm_area_failed;
        alloc->buffer = area->addr;
        //用户空间地址相对于内核地址的差值
        alloc->user_buffer_offset =vma->vm_start - (uintptr_t)alloc->buffer;
        mutex_unlock(&binder_alloc_mmap_lock);

        #ifdef CONFIG_CPU_CACHE_VIPT
        if (cache_is_vipt_aliasing()) {
            while (CACHE_COLOUR(
                    (vma->vm_start ^ (uint32_t)alloc->buffer))) {
                pr_info("binder_mmap: %d %lx-%lx maps %pK bad alignment\n",
                    alloc->pid, vma->vm_start, vma->vm_end,
                    alloc->buffer);
                vma->vm_start += PAGE_SIZE;
            }
        }
        #endif
        //分配pages数组内存
        alloc->pages = kzalloc(sizeof(alloc->pages[0]) *
                       ((vma->vm_end - vma->vm_start) / PAGE_SIZE),
                       GFP_KERNEL);
        if (alloc->pages == NULL) 
            ret = -ENOMEM;
            failure_string = "alloc page array";
            goto err_alloc_pages_failed;
        alloc->buffer_size = vma->vm_end - vma->vm_start;//mmap指定的buffer大小

        //分配一个binder_buffer内存,用于binder交易事物
        buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
        if (!buffer) 
            ret = -ENOMEM;
            failure_string = "alloc buffer struct";
            goto err_alloc_buf_struct_failed;

        buffer->data = alloc->buffer;
        list_add(&buffer->entry, &alloc->buffers);
        buffer->free = 1;
        binder_insert_free_buffer(alloc, buffer);
        //异步事务的缓冲区大小是总的一半
        alloc->free_async_space = alloc->buffer_size / 2;
        barrier();
        alloc->vma = vma;
        alloc->vma_vm_mm = vma->vm_mm;
        /* Same as mmgrab() in later kernel versions */
        atomic_inc(&alloc->vma_vm_mm->mm_count);
        return 0;

	if (ret)
		return ret;
	mutex_lock(&proc->files_lock);
	proc->files = get_files_struct(current);
	mutex_unlock(&proc->files_lock);
	return 0;

err_bad_arg:
	pr_err("%s: %d %lx-%lx %s failed %d\n", __func__,
	       proc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
	return ret;


//binder_buffer的管理，即申请和释放
//申请buffer
binder_alloc_new_buf
//释放buffer
binder_alloc_free_buf






















