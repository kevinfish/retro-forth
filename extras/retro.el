;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; retro-mode for emacs
;; version: 0.01
;;
;; ----------------------------------------------------------
;; Copyright (c) 2012 Michal J Wallace <michal.wallace@gmail.com>
;;
;; Permission to use, copy, modify, and/or distribute this software
;; for any purpose with or without fee is hereby granted, provided
;; that the above copyright notice and this permission notice appear
;; in all copies.
;;
;; THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
;; WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
;; WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
;; AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
;; CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
;; LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
;; NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
;; CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
;; ----------------------------------------------------------
;; ( http://en.wikipedia.org/wiki/ISC_license )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; usage : add this to your .emacs file:
;;
;; (autoload 'retro-mode "retro-mode.el" "retro mode" t)
;; (add-to-list 'auto-mode-alist '("\\.rx\\'" . retro-mode))



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; changelog
;;
;; 2012-10-09 v. 0.1 mjw
;; added isc license, updated keywords, added usage, changelog
;; failed attempted to get doc{ ... }doc comments and prefixes 
;; to highlight correctly.
;;
;; 2012-07-28 v. 0.0 mjw
;; retro-mode for emacs based on //retro/examples/retro.vim
;; limited to basic syntax highlighting
;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; implementation
;;

(require 'generic-x)

;----------------------------------------------------------
; syntax table :
;
; by default, make everything a word constituent, (rather
; than punctuation or a quoting character).
;
; This is is because retro uses lots of odd characters as
; parts of a word, and we want to match them in our regexps.
; ref: http://www.emacswiki.org/emacs/EmacsSyntaxTable

(defvar retro-table)
(setq retro-table
      (make-char-table 'syntax-table (string-to-syntax "w")))

;; whitespace characters:
(modify-syntax-entry 10 "-" retro-table) ; cr
(modify-syntax-entry 13 "-" retro-table) ; lf
(modify-syntax-entry 32 "-" retro-table) ; space

;; I thought this might help with prefixes, but it doesn't. :/
;; (modify-syntax-entry 33 "_" retro-table ) ; !


;----------------------------------------------------------
; face definitions for syntax highlighting
; ( instead of editing the colors here, use M-x customize-face )
;
(defface retro-opword-face
  '((default (:foreground "cyan" )))
  "Face for math/logic operators in retro.")

(defface retro-memword-face
  '((default (:foreground "cyan" )))
  "Face for memory-related words in retro.")

(defface retro-stackword-face
  '((default (:foreground "cyan" )))
  "Face for stack-related words in retro.")

(defface retro-condword-face
  '((default (:foreground "gold" :bold t )))
  "Face for condition-related words in retro.")

(defface retro-flowword-face
  '((default (:foreground "gold" :bold t )))
  "Face for loop-related words in retro.")

(defface retro-defword-face
  '((default (:foreground "#c63" :bold t)))
  "Face for defining words in retro.")

(defface retro-number-face
  '((default (:foreground "limegreen")))
  "Face for numbers in retro.")

(defface retro-primitive-face
  '((default (:background "#116" :foreground "#99f" )))
  "Face for primitives in retro.")

(defface retro-variable-face
  '((default (:foreground "#ff6" )))
  "Face for variable definitions in retro.")

(defface retro-prefix-face
  '((default (:foreground "#099" )))
  "Face for prefixes in retro.")

(defface retro-module-face
  '((default (:foreground "tomato")))
  "Face for module names in retro.")


;----------------------------------------------------------
; generic functions to make it easy to define all the
; keywords below

(defun word-bound (rx)
  "wrap the regexp in word-boundary markers"
  (concat "\\<" rx "\\>" ))

(defun between-words (a b)
  "match anything between words a and b, including those words"
  (concat (word-bound a) ".*?"  (word-bound b)))

(defun make-facewords (faceword-alist)
  "[ ( face , [ word ] ) ] -> "
  (let ((facewords
         (lambda (face words)
           (mapcar (lambda (word)
                     (cons (word-bound
                            (if (stringp word) word
                              (regexp-quote (symbol-name word))))
                           face))
                   words))))
    (apply 'nconc (mapcar (lambda (alist) (apply facewords alist))
                          faceword-alist))))


;; this is just a test for the code above. you can safely remove
;; it if it causes problems for you.
(require 'cl) ;; assert is part of the common lisp module
(assert (equal
	   '(("\\<\\+\\>" . 'f0)
	     ("\\<b\\>" . 'f0)
	     ("\\<c\\>" . 'f0)
	     ("\\<x\\>" . 'f1)
	     ("\\<y\\>" . 'f1)
	     ("\\<z\\>" . 'f1))
	   (make-facewords
	    '(('f0 (+ b c))
	      ('f1 ("x" "y" "z"))))))
(assert (equal '(("\\<[a-z]+:\\>" quote f0))
	       (make-facewords '(('f0 ("[a-z]+:" ))))))




;----------------------------------------------------------
; retro-mode

(define-generic-mode
    'retro-mode

  ;; comment chars normally go here, but in retro, it's not the characters ( )
  ;; that make a comment, but rather the *words* "(" and ")" .. we need a
  ;; full regexp to do proper word boundary matching
  '( )

  ;; keywords would normally go here, but a) retro doesn't really have keywords
  ;; and b) we want comments to be the first thing to match so other things
  ;; don't get highlighted inside them
  '( )

  ;; syntax highlighting. these were taken from
  (make-facewords

   `(
     ('font-lock-comment-face
      (list ,(between-words "(" ")")))

     ; todo ,(between-words "doc{" "}doc") - generic-mode doesn't support multi-line

     ('font-lock-keyword-face
      ,(mapcar 'regexp-quote '("[" "[[" "]]" "]")))

     ('retro-number-face
      (list "-?[0-9][0-9a-fA-F]*"))

     ('font-lock-string-face
      (list "\\\".*?\\\""))

     ('retro-assembler-face
      (list "nop," "dup," "drop," "swap," "push," "pop," "loop," "jump,"
	    "ret," ";," ">jump," "<jump," "!jump," "=jump," "@," "!,"
	    "+," "-," "*," "/mod," "and," "or," "xor," "<<," ">>,"
	    ; "0;" this primitive makes no sense to use anywhere but inside
	    ; the compiler, so we render it like ; rather than like ;,/ret,
	    "1+," "1-," "in," "out," "wait," ))

     ('retro-opword-face
      (list + - * / mod /mod neg abs min
            and or xor not << >> 1+ 1- ++ --
            < <> = > <= >= within getLength withLength compare ))

     ('retro-stackword-face
      (list drop nip dup over tuck swap rot -rot tib
            dip sip bi bi@ bi* tri tri@ tri* each each@
            2dup 2drop
            push pop r rdrop))

     ('retro-memword-face
      (list "@" ! +! @+ !+ -! on off
            keepString tempString accept
            "#mem" allot here heap
            fill copy))

     ('retro-condword-face
      (list "if" "if;" else ";then" then ahead when whend
            =if <if !if >if ifTrue ifFalse))

     ('retro-module-face
      (list "\\w+'"))

     ('retro-flowword-face
      (list repeat again ";;" "0;" times iter iterd while for next ))


     ('retro-prefix-face
      (list "@")) ;[@$!#&]+"))

     ; todo : how to match variables inside the variables| ... | ?
     ; todo : what about special prefixes?
     ; ('retro-variable-face  (list ))

     ('retro-defword-face
      (list "[a-z]+:" "'" ":" ";[a-z]*" with-class
            ".word" ".macro" ".data" ".primitive"
            eval ifNotDefined ifDefined
            constant variable variable: elements
            create value "\\:is" is immediate
            compile-only compile "`" do
            literal literal, ":devector" devector
            find "," decimal hex octal binary
            "{{" "}}" "---reveal---"
            "with" "without" "needs" "global" "save" "bye"
	    | variables| elements|
            ))))

  ;; auto-mode list (filename matchrs)
  '("\\.rx$" )

  ;; startup hook
  (list (lambda () (set-syntax-table retro-table)))

  ;; docstring
  "A rudimentary major mode for editing retro files" )

;; end.
