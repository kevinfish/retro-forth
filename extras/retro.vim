" Vim syntax file
"
" Language:   Retro
" Comments:   Based on the ANS Forth syntax file bundled with Vim.
"               * Same general structure; overhaul of keywords.
" Modifier:   Marc Simpson
"
" --------------------------------------------------------------------
"  Original ackowledgements/changes for ANS version.
" --------------------------------------------------------------------
" Vim syntax file
" Language:    FORTH
" Maintainer:  Christian V. J. Brüssow <cvjb@cvjb.de>
" Last Change: Sa 09 Feb 2008 13:27:29 CET
" Filenames:   *.fs,*.ft
" URL:         http://www.cvjb.de/comp/vim/forth.vim

" $Id: forth.vim,v 1.11 2008/02/09 13:17:01 bruessow Exp $

" The list of keywords is incomplete, compared with the offical ANS
" wordlist. If you use this language, please improve it, and send me
" the patches.
"
" Before sending me patches, please download the newest version of
" this file from http://www.cvjb.de/comp/vim/forth.vim or
" http://www.vim.org/ (search for forth.vim).

" Many Thanks to...
"
" 2008-02-09:
" Shawn K. Quinn <sjquinn at speakeasy dot net> send a big patch
" with new words commonly used in Forth programs or defined by GNU
" Forth.
"
" 2007-07-11:
" Benjamin Krill <ben at codiert dot org> send me a patch to
" highlight space errors.  You can toggle this feature on through
" setting the flag forth_space_errors in you vimrc. If you have
" switched it on, you can turn off highlighting of trailing spaces
" in comments by setting forth_no_trail_space_error in your vimrc.
" If you do not want the highlighting of a tabulator following a
" space in comments, you can turn this off by setting
" forth_no_tab_space_error.
"
" 2006-05-25:
" Bill McCarthy <WJMc@...> and Ilya Sher <ilya-vim@...>
" Who found a bug in the ccomment line in 2004!!!
" I'm really very sorry, that it has taken two years to fix that in
" the offical version of this file. Shame on me.  I think my face
" will be red the next ten years...
"
" 2006-05-21:
" Thomas E. Vaughan <tevaugha at ball dot com> send me a patch for
" the parenthesis comment word, so words with a trailing parenthesis
" will not start the highlighting for such comments.
"
" 2003-05-10:
" Andrew Gaul <andrew at gaul.org> send me a patch for
" forthOperators.
"
" 2003-04-03:
" Ron Aaron <ron at ronware dot org> made updates for an improved
" Win32Forth support.
"
" 2002-04-22:
" Charles Shattuck <charley at forth dot org> helped me to settle up
" with the binary and hex number highlighting.
"
" 2002-04-20:
" Charles Shattuck <charley at forth dot org> send me some code for
" correctly highlighting char and [char] followed by an opening
" paren. He also added some words for operators, conditionals, and
" definitions; and added the highlighting for s" and c".
"
" 2000-03-28:
" John Providenza <john at probo dot com> made improvements for the
" highlighting of strings, and added the code for highlighting hex
" numbers.
"
" --------------------------------------------------------------------

" --------------------------------------------------------------------
"  Preamble
" --------------------------------------------------------------------

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
    syntax clear
elseif exists("b:current_syntax")
    finish
endif

" Synchronization method
syn sync ccomment
syn sync maxlines=200

" Comment out case insensitive matching:
" syn case ignore

" Some special, non-FORTH keywords
syn keyword forthTodo contained TODO FIXME XXX
" syn match forthTodo contained 'Copyright\(\s([Cc])\)\=\(\s[0-9]\{2,4}\)\='

" Characters allowed in keywords
" I don't know if 128-255 are allowed in ANS-FORTH
if version >= 600
    setlocal iskeyword=!,@,33-35,%,$,38-64,A-Z,91-96,a-z,123-126,128-255
else
    set iskeyword=!,@,33-35,%,$,38-64,A-Z,91-96,a-z,123-126,128-255
endif

" when wanted, highlight trailing white space
if exists("forth_space_errors")
    if !exists("forth_no_trail_space_error")
        syn match forthSpaceError display excludenl "\s\+$"
    endif
    if !exists("forth_no_tab_space_error")
        syn match forthSpaceError display " \+\t"me=e-1
    endif
endif

" --------------------------------------------------------------------
"  Keywords
" --------------------------------------------------------------------

" basic mathematical and logical operators
syn keyword forthOperators + - * / mod /mod neg abs min max
syn keyword forthOperators and or xor not << >> 1+
syn keyword forthOperators 1-  ++ --
syn keyword forthOperators < <> = >
syn keyword forthOperators within getLength withLength compare

" stack manipulations
syn keyword forthStack drop nip dup over tuck swap rot -rot
syn keyword forthStack dip sip bi bi@ bi* tri tri@ tri* each each@
syn keyword forthStack 2dup 2drop
syn keyword forthRStack push pop r rdrop

" stack pointer manipulations
" syn keyword forthSP SP@ SP! FP@ FP! RP@ RP! LP@ LP!

" address operations
syn keyword forthMemory @ ! +! @+ !+ -! on off
syn keyword forthMemory keepString tempString accept
syn keyword forthMemory #mem
syn keyword forthAdrArith allot here zallot heap
syn keyword forthMemBlks fill copy

" conditionals
syn keyword forthCond if if; else ;then then ahead when whend
syn keyword forthCond =if <if !if >if ifTrue ifFalse

" iterations
syn keyword forthLoop repeat again
syn keyword forthLoop ;; 0; times iter iterd while

" new words
syn match forthColonDef '\<:m\?\s*[^ \t]\+\>'
syn keyword forthEndOfColonDef ;
syn keyword forthDefine with-class .word .macro .data
syn keyword forthDefine eval ifNotDefined ifDefined
syn keyword forthDefine constant variable variable: elements
syn keyword forthDefine create value :is is does> immediate
syn keyword forthDefine compile-only compile ` do
syn keyword forthDefine literal literal,
syn keyword forthDefine :devector devector
" Quote highlighting leads to too much colour---re-enable if you disagree.
"syn keyword forthDefine ]
"syn match forthDefine '\s\(\[\s\)\+'
"syn match forthDefine '^\[\s'
syn keyword forthDefine find
syn keyword forthDefine ,
syn region forthDefine start='variables|' end='|'


" basic character operations (char: is handled separately with a match)
syn keyword forthCharOps putn space find puts putc getc
syn keyword forthCharOps tib cr

" __' prefix for characters
syn match forthCharOps '\'.\s'

" char-number conversion
syn keyword forthConversion toNumber toString

" interptreter, wordbook, compiler
syn keyword forthForth bye here clear save wait in out words
syn keyword forthForth d->class d->name d->xt xt->d forget

" vocabularies
syn keyword forthVocs {{ ---reveal--- }}  chain: ;chain
syn keyword forthVocs global with without %% <%>
" syn keyword forthVocs ['] '

" File keywords
syn keyword forthBlocks set-blocks new e ea offset blk block #-blocks

" numbers
syn keyword forthMath decimal hex octal binary base
syn match forthInteger '\<-\=[0-9.]*[0-9.]\+\>'
syn match forthInteger '\<&-\=[0-9.]*[0-9.]\+\>'
" recognize hex and binary numbers, the '$' and '%' notation is for gforth
syn match forthInteger '\<\$\x*\x\+\>' " *1* --- dont't mess
syn match forthInteger '\<\x*\d\x*\>'  " *2* --- this order!
syn match forthInteger '\<%[0-1]*[0-1]\+\>'

" Strings ("foo")
syn region forthString start=+\.*\"+ end=+"+
" end=+$+

" Comments
syn region forthComment start='\(\s\|^\)(\s' skip='\\)' end=')' end='$' contains=forthTodo,forthSpaceError

" Include files
syn match forthInclude '^include\s\+\k\+'

" --------------------------------------------------------------------
"  Define the default highlighting.
" --------------------------------------------------------------------

" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_forth_syn_inits")
    if version < 508
  let did_forth_syn_inits = 1
  command -nargs=+ HiLink hi link <args>
    else
  command -nargs=+ HiLink hi def link <args>
    endif

    " The default methods for highlighting. Can be overriden later.
    HiLink forthTodo Todo
    HiLink forthOperators Operator
    HiLink forthMath Number
    HiLink forthInteger Number
    HiLink forthStack Special
    HiLink forthRstack Special
    HiLink forthSP Special
    HiLink forthMemory Function
    HiLink forthAdrArith Function
    HiLink forthMemBlks Function
    HiLink forthCond Conditional
    HiLink forthLoop Repeat
    HiLink forthColonDef Define
    HiLink forthEndOfColonDef Define
    HiLink forthDefine Define
    HiLink forthDebug Debug
    HiLink forthAssembler Include
    HiLink forthCharOps Character
    HiLink forthConversion String
    HiLink forthForth Statement
    HiLink forthVocs Statement
    HiLink forthString String
    HiLink forthComment Comment
    HiLink forthClassDef Define
    HiLink forthEndOfClassDef Define
    HiLink forthObjectDef Define
    HiLink forthEndOfObjectDef Define
    HiLink forthInclude Include
    HiLink forthLocals Type " nothing else uses type and locals must stand out
    HiLink forthDeprecated Error " if you must, change to Type
    HiLink forthFileMode Function
    HiLink forthBlocks Statement
    HiLink forthSpaceError Error

    delcommand HiLink
endif

let b:current_syntax = "forth"
