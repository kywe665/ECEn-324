call pathogen#infect()
syntax on
set expandtab
set shiftwidth=2
set tabstop=2
set smarttab
set number

colorscheme desert

set nohlsearch          " turn off highlight searches, but:
                        " Turn hlsearch off/on with CTRL-N
:map <silent> <C-H> :se invhlsearch<CR>

" Search for selected text, forwards or backwards.
vnoremap <silent> * :<C-U>
  \let old_reg=getreg('"')<Bar>let old_regtype=getregtype('"')<CR>
  \gvy/<C-R><C-R>=substitute(
  \escape(@", '/\.*$^~['), '\_s\+', '\\_s\\+', 'g')<CR><CR>
  \gV:call setreg('"', old_reg, old_regtype)<CR>
vnoremap <silent> # :<C-U>
  \let old_reg=getreg('"')<Bar>let old_regtype=getregtype('"')<CR>
  \gvy?<C-R><C-R>=substitute(
  \escape(@", '?\.*$^~['), '\_s\+', '\\_s\\+', 'g')<CR><CR>
  \gV:call setreg('"', old_reg, old_regtype)<CR>

:noremap <2-LeftMouse> *
:inoremap <2-LeftMouse> <c-o>*

inoremap <c-u> <c-g>u<c-u>
inoremap <c-w> <c-g>u<c-w>

source $VIMRUNTIME/mswin.vim

vmap <Tab> >gv
vmap <S-Tab> <gv

