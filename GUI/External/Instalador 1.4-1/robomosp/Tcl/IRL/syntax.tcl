#  syntax highlighting code

set syntaxList {}

set procExprArray() {}
set commentSymbolArray() {}
set commentOpenArray() {}
set commentCloseArray() {}
set commentRangeArray() {}
set commentExprArray() {}
set keywordRangeArray() {}
set keywordExprArray() {}

unset procExprArray()
unset commentSymbolArray()
unset commentOpenArray()
unset commentCloseArray()
unset commentRangeArray()
unset commentExprArray()
unset keywordRangeArray()
unset keywordExprArray()

proc SetIndent {} {

	# check indent button and bind accoridingly
	
	if {[GetDefault indentButton]} then {
		bind indent <KeyPress-Return> {IndentBind %W}
	} else {
		bind indent <KeyPress-Return> {}
	}
}

proc SetComment {} {

	# check comment button and bind accordingly
	
	if {[GetDefault commentButton]} then {
		bind comment <KeyPress> {CommentBind %W}
	} else {
		bind comment <KeyPress> {}
	}
}

proc SetKeyword {} {

	# check keyword button and bind accordingly
	
	if {[GetDefault keywordButton]} then {
		bind keyword <KeyPress> {KeywordBind %W}
	} else {
		bind keyword <KeyPress> {}
	}
}

proc ReadSyntax {} {

	# globalize

	global directory
	global syntaxList
	global procExprArray	
	global commentSymbolArray
	global commentOpenArray
	global commentCloseArray
	global commentRangeArray
	global commentExprArray
	global keywordRangeArray
	global keywordExprArray
	
	# intalize file name
	
	set file [file join $directory syntax.txt]
	
	# open the file stream

	if {[catch {open $file r} stream]} then {
		tk_messageBox -type ok -icon error -message "Unable to read $file"
		return
	}
	
	# first line contains list of supported languages
	
	gets $stream syntaxList
	
	# loop through list of languages and initalize arrays
	
	foreach syntax $syntaxList {
		
		# initalize the arrays
		
		gets $stream procExprArray($syntax)
		gets $stream commentSymbolArray($syntax)
		gets $stream commentOpenArray($syntax)
		gets $stream commentCloseArray($syntax)
		gets $stream commentRangeArray($syntax)
		gets $stream commentExprArray($syntax)
		gets $stream keywordRangeArray($syntax)
		gets $stream keywordExprArray($syntax)
	}
	
	# close the file stream
	
	close $stream
}

proc HighlightText {firstIndex lastIndex} {

	# globalize
	
	global commentOpenArray
	global commentCloseArray
	global commentRangeArray
	global commentExprArray
	global keywordRangeArray
	global keywordExprArray
	
	# initalize expressions
	
	set commentOpen $commentOpenArray([GetDefault syntaxButton])
	set commentClose $commentCloseArray([GetDefault syntaxButton])
	set commentRange $commentRangeArray([GetDefault syntaxButton])
	set commentExpr $commentExprArray([GetDefault syntaxButton])
	set keywordRange $keywordRangeArray([GetDefault syntaxButton])
	set keywordExpr $keywordExprArray([GetDefault syntaxButton])
	
	# strip text of all tags
	
	set text [GetText]
	$text tag remove comment $firstIndex $lastIndex
	$text tag remove keyword $firstIndex $lastIndex
	
	# determine lines
	
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine
	
	# check comment button
	
	if {[GetDefault commentButton]} then {
	
		# loop through specified lines
	
		for {set count $firstLine} {$count <= $lastLine} {incr count} {
		
			# get line indices
			
			set lineStart $count.0
			set lineStop [$text index "$count.0 lineend"]
			
			# initalize variables
			
			set searchStart $lineStart
			set searchStop $lineStop
		
			# check for line comment and highlight if found
			
			set rangeIndex [$text search -count rangeLength -regexp -- $commentRange $searchStart $searchStop]
			if {[string compare $rangeIndex {}] != 0} then {
			
				# seach for comment within comment range
				
				set rangeStart $rangeIndex
				set rangeStop [$text index "$rangeIndex + $rangeLength chars"]
				set commentIndex [$text search -count commentLength -regexp -- $commentExpr $rangeStart $rangeStop]
				
				# highlight comment found within range
				
				set commentStart $commentIndex
				set commentStop [$text index "$commentIndex + $commentLength chars"]
				$text tag add comment $commentStart $commentStop
			}
		}
	}
	
	# check keyword button
	
	if {[GetDefault keywordButton]} then {
	
		# loop through specified lines
	
		for {set count $firstLine} {$count <= $lastLine} {incr count} {
		
			# get line indices
			
			set lineStart $count.0
			set lineStop [$text index "$count.0 lineend"]
			
			# initalize variables
			
			set searchStart $lineStart
			set searchStop $lineStop
			
			# check for keywords and highlight if found
			
			while {1} {
			
				set rangeIndex [$text search -count rangeLength -regexp -- $keywordRange $searchStart $searchStop]
				if {[string compare $rangeIndex {}] == 0} then {
					break
				}
				
				# seach for keyword within keyword range
				
				set rangeStart $rangeIndex
				set rangeStop [$text index "$rangeIndex + $rangeLength chars"]
				set keywordIndex [$text search -count keywordLength -regexp -- $keywordExpr $rangeStart $rangeStop]
				
				# highlight comment found within range
				
				set keywordStart $keywordIndex
				set keywordStop [$text index "$keywordIndex + $keywordLength chars"]
				$text tag add keyword $keywordStart $keywordStop
				
				# update search start
				
				set searchStart $keywordStop
			}
		}
	}
}

proc IndentBind {text} {

	# check for tabs

	set tabIndex [$text search -count tabCount -regexp -- {^\t*} {insert - 1 line linestart} {insert -1 line lineend}]
	if {[string compare $tabIndex {}] != 0} then {
	
		# insert same number tabs in next line rembering undo
	
		for {set count 0} {$count < $tabCount} {incr count} {
			AppendUndo $text delete [$text index insert] {}
			$text insert insert \t
		}
	}
}

proc CommentBind {text} {

	# globalize
	
	global commentOpenArray
	global commentCloseArray
	global commentRangeArray
	global commentExprArray
	
	# initalize expressions
	
	set commentOpen $commentOpenArray([GetDefault syntaxButton])
	set commentClose $commentCloseArray([GetDefault syntaxButton])
	set commentRange $commentRangeArray([GetDefault syntaxButton])
	set commentExpr $commentExprArray([GetDefault syntaxButton])
	
	# get line indicies
	
	set lineStart [$text index {insert linestart}]
	set lineStop [$text index {insert lineend}]

	# strip all comment tags from line

	$text tag remove comment $lineStart $lineStop
	
	# initalize variables
	
	set searchStart $lineStart
	set searchStop $lineStop

	# check for line comment and highlight if found
	
	set rangeIndex [$text search -count rangeLength -regexp -- $commentRange $searchStart $searchStop]
	if {[string compare $rangeIndex {}] != 0} then {
	
		# seach for comment within comment range
		
		set rangeStart $rangeIndex
		set rangeStop [$text index "$rangeIndex + $rangeLength chars"]
		set commentIndex [$text search -count commentLength -regexp -- $commentExpr $rangeStart $rangeStop]
		
		# highlight comment found within range
		
		set commentStart $commentIndex
		set commentStop [$text index "$commentIndex + $commentLength chars"]
		$text tag add comment $commentStart $commentStop
	}
}

proc KeywordBind {text} {

	# globalize
	
	global keywordRangeArray
	global keywordExprArray
	
	# initalize expressions
	
	set keywordRange $keywordRangeArray([GetDefault syntaxButton])
	set keywordExpr $keywordExprArray([GetDefault syntaxButton])
	
	# get line indicies
	
	set lineStart [$text index {insert linestart}]
	set lineStop [$text index {insert lineend}]

	# strip all keyword tags from line

	$text tag remove keyword $lineStart $lineStop
	
	# initalize variables
	
	set searchStart $lineStart
	set searchStop $lineStop
	
	# check for keywords and highlight if found
	
	while {1} {
	
		set rangeIndex [$text search -count rangeLength -regexp -- $keywordRange $searchStart $searchStop]
		if {[string compare $rangeIndex {}] == 0} then {
			break
		}
		
		# seach for keyword within keyword range
		
		set rangeStart $rangeIndex
		set rangeStop [$text index "$rangeIndex + $rangeLength chars"]
		set keywordIndex [$text search -count keywordLength -regexp -- $keywordExpr $rangeStart $rangeStop]
		
		# highlight comment found within range
		
		set keywordStart $keywordIndex
		set keywordStop [$text index "$keywordIndex + $keywordLength chars"]
		$text tag add keyword $keywordStart $keywordStop
		
		# update search start
		
		set searchStart $keywordStop
	}
}
