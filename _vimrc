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
set ignorecase

" 遍历窗口
nnoremap nw <C-W><C-W>
nnoremap <Leader>lw <C-W>l
nnoremap <Leader>hw <C-W>h
nnoremap <Leader>kw <C-W>k
nnoremap <Leader>jw <C-W>j

" 禁止光标闪烁
set gcr=a:block-blinkon0

" 禁止显示滚动条
set guioptions-=l
set guioptions-=L
set guioptions-=r
set guioptions-=R
" 禁止显示菜单和工具条
set guioptions-=m
set guioptions-=T

" 总是显示状态栏
set laststatus=2

" 显示光标当前位置
set ruler

" 开启行号显示
set number

" 高亮显示当前行/列
set cursorline
set cursorcolumn

" 高亮显示搜索结果
set hlsearch

" 禁止折行
set nowrap

" 设置状态栏主题风格
let g:Powerline_colorscheme='solarized256'

" 开启语法高亮功能
syntax enable
" 允许用指定语法高亮配色方案替换默认方案
syntax on

" 自适应不同语言的智能缩进
filetype indent on

" 将制表符扩展为空格
set expandtab
" 设置编辑时制表符占用空格数
set tabstop=4
" 设置格式化时制表符占用空格数
set shiftwidth=4
" 让 vim 把连续数量的空格视为一个制表符
set softtabstop=4

set cindent
set autoindent
set smartindent

" Vundle配置
set nocompatible
filetype off
set rtp+=$VIM/vimfiles/bundle/Vundle.vim/
 
call vundle#begin('$VIM/vimfiles/bundle/')
Plugin 'VundleVim/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'scrooloose/nerdcommenter'
call vundle#end()
 
filetype plugin indent on

" nerdcommenter插件，自动注释
"<leader>cc，注释当前选中文本，如果选中的是整行则在每行首添加 //，如果选中一行的部分内容则在选中部分前后添加分别 /、/；
"<leader>cu，取消选中文本块的注释。

" nerdtree插件操作
nmap <Leader>t :NERDTreeToggle<CR>
" 设置 NERDTree 子窗口宽度
let NERDTreeWinSize=22
" 设置 NERDTree 子窗口位置
let NERDTreeWinPos="right"
" 显示隐藏文件
let NERDTreeShowHidden=1
" NERDTree 子窗口中不显示冗余帮助信息
let NERDTreeMinimalUI=1
" 删除文件时自动删除文件对应 buffer
let NERDTreeAutoDeleteBuffer=1
