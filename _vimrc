set encoding=utf-8

let mapleader=";"
filetype on
filetype plugin on

autocmd BufWritePost $MYVIMRC source $MYVIMRC
" map: 所有模式生效
" nmap: normal模式生效
" nore: 表示非递归，不对映射继续映射

colorscheme evening

set incsearch
set ignorecase smartcase

nnoremap <Leader>nw <C-W><C-W> " 遍历窗口
nnoremap <Leader>lw <C-W>l
nnoremap <Leader>hw <C-W>h
nnoremap <Leader>kw <C-W>k
nnoremap <Leader>jw <C-W>j

set gcr=a:block-blinkon0 " 禁止光标闪烁

set guioptions-=l " 禁止显示滚动条
set guioptions-=L
set guioptions-=r
set guioptions-=R

set guioptions-=m " 禁止显示菜单和工具条
set guioptions-=T

set laststatus=2 " 总是显示状态栏

set ruler " 显示光标当前位置
set number " 开启行号显示
set cursorline " 高亮显示当前行/列
set cursorcolumn
set hlsearch " 高亮显示搜索结果
set nowrap " 禁止折行
let g:Powerline_colorscheme='solarized256' " 设置状态栏主题风格

syntax enable " 开启语法高亮功能
syntax on " 允许用指定语法高亮配色方案替换默认方案
filetype indent on " 自适应不同语言的智能缩进

set expandtab " 将制表符扩展为空格
set tabstop=4 " 设置编辑时制表符占用空格数
set shiftwidth=4 "设置格式化时制表符占用空格数
set softtabstop=4 "让 vim 把连续数量的空格视为一个制表符
"set autochdir "自动设置当前编辑文件目录为工作路径

:set cindent
"set autoindent
"set smartindent

" Vundle配置
set nocompatible
filetype off
"Windows
"set rtp+=$VIM/vimfiles/bundle/Vundle.vim/
"call vundle#begin('$VIM/vimfiles/bundle/')
"Linux
set rtp+=~/.vim/bundle/Vundle.vim/
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'scrooloose/nerdcommenter'
Plugin 'kien/ctrlp.vim'
Plugin 'dyng/ctrlsf.vim'
Plugin 'majutsushi/tagbar'
Plugin 'fholgado/minibufexpl.vim'
call vundle#end()
 
filetype plugin indent on

" nerdcommenter插件，自动注释
"<leader>cc，注释当前选中文本，如果选中的是整行则在每行首添加 //，如果选中一行的部分内容则在选中部分前后添加分别 /、/；
"<leader>cu，取消选中文本块的注释。

" nerdtree插件操作
nmap <Leader>tt :NERDTreeToggle<CR>
nmap <Leader>tc :NERDTreeCWD<CR> "切换nerdtree根目录为当前working dir

let NERDTreeWinSize=32 " 设置 NERDTree 子窗口宽度
let NERDTreeWinPos="right" " 设置 NERDTree 子窗口位置
let NERDTreeShowHidden=1 " 显示隐藏文件
let NERDTreeMinimalUI=1 " NERDTree 子窗口中不显示冗余帮助信息
let NERDTreeAutoDeleteBuffer=1 " 删除文件时自动删除文件对应 buffer

" ctrlsf操作
nmap <Leader>sp <Plug>CtrlSFPrompt

"ctags操作
"ctags -R 扫描当前目录和子目录
":set tags+=/path/to/tags
"CTRL+] 直接跳转, g] 列出所有标签, CTRL+t 返回 

"tagbar settings
nmap <Leader>tb :TagbarToggle<CR>
let tagbar_left=1
let tagbar_width=32

"minibufferexpl
nmap <Leader><Tab> :MBEbn<cr>
nmap <Leader>b :MBEbp<cr>


