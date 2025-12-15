# window manipulation functions

set textList {}
set yScrollList {}
set xScrollList {}
set undoList {}
set redoList {}
set fileList {}
set activeWindow {}
set widgetCount 0
set undoArray() {}
set redoArray() {}
unset redoArray()
unset undoArray()

proc CreateWindow {file} {

	# globalize

	global fileList	
	global textList
	global yScrollList
	global xScrollList
	global undoList
	global redoList
	global widgetCount
	global activeWindow
	global undoArray
	global redoArray

	# initalize local variables

	set text .text$widgetCount
	set yScroll .yscroll$widgetCount
	set xScroll .xscroll$widgetCount
	set undo undo$widgetCount
	set redo redo$widgetCount
	set undoArray(undo$widgetCount) {}
	set redoArray(redo$widgetCount) {}
	
	# increment window count
	
	incr widgetCount 1

	# append widget names to lists

	lappend fileList $file
	lappend textList $text
	lappend yScrollList $yScroll
	lappend xScrollList $xScroll
	lappend undoList $undo
	lappend redoList $redo
	

	# initalize activeWindow

	set activeWindow [expr [llength $textList] - 1]

	# create widgets

	text $text
	scrollbar $yScroll -orient vertical
	scrollbar $xScroll -orient horizontal

	# configure text widget

	$text config -font [GetDefault rootFont]
	$text config -background [GetDefault backgroundColor]
	$text config -foreground [GetDefault foregroundColor]
	$text config -insertbackground [GetDefault foregroundColor]
	$text config -wrap [GetDefault wrapButton]
	$text config -tabs [GetDefault tabStops]

	# configure text tags

	$text tag config sel -background [GetDefault foregroundColor]
	$text tag config sel -foreground [GetDefault backgroundColor]
	$text tag config comment -font [GetDefault commentFont]
	$text tag config comment -foreground [GetDefault commentColor]
	$text tag config keyword -font [GetDefault keywordFont]
	$text tag config keyword -foreground [GetDefault keywordColor]
	
	# comment tag should have precedence over keyword tag and sel over all
	
	$text tag raise comment
	$text tag raise sel
	
	# configure bindtags
	
	bindtags $text [list $text undo Text indent comment keyword . all]

	# configure scrolling behaviour

	$text config -yscrollcommand "$yScroll set"
	$text config -xscrollcommand "$xScroll set"
	$yScroll config -command "$text yview"
	$xScroll config -command "$text xview"
}

proc GridWindow {} {

	# globalize

	global textList
	global yScrollList
	global xScrollList
	global activeWindow

	# initalize variables

	set text [lindex $textList $activeWindow]
	set yScroll [lindex $yScrollList $activeWindow]
	set xScroll [lindex $xScrollList $activeWindow]

	# display newly created widgets

	grid $text -row 1 -column 0 -sticky news
	grid $yScroll -row 1 -column 1 -sticky ns
	grid $xScroll -row 2 -column 0 -columnspan 2 -sticky we

	# take care of interface

	WindowsMenu
	FilebarList
	focus $text
}

proc RaiseWindow {} {

	# globalize

	global textList
	global yScrollList
	global xScrollList
	global activeWindow

	# initalize variables

	set text [lindex $textList $activeWindow]
	set yScroll [lindex $yScrollList $activeWindow]
	set xScroll [lindex $xScrollList $activeWindow]

	# raise the spedified widgets

	raise $text
	raise $yScroll
	raise $xScroll

	# take care of interface

	WindowsMenu
	FilebarList
	focus $text
}

proc DestroyWindow {} {

	# globalize

	global fileList
	global textList
	global yScrollList
	global xScrollList
	global undoList
	global redoList
	global activeWindow
	global undoArray
	global redoArray

	# initalize variables from lists

	set file [lindex $fileList $activeWindow]
	set text [lindex $textList $activeWindow]
	set yScroll [lindex $yScrollList $activeWindow]
	set xScroll [lindex $xScrollList $activeWindow]
	set undo [lindex $undoList $activeWindow]
	set redo [lindex $redoList $activeWindow]
	

	# remove widgets from lists

	set textList [lreplace $textList $activeWindow $activeWindow]
	set yScrollList [lreplace $yScrollList $activeWindow $activeWindow]
	set xScrollList [lreplace $xScrollList $activeWindow $activeWindow]
	set undoList [lreplace $undoList $activeWindow $activeWindow]
	set redoList [lreplace $redoList $activeWindow $activeWindow]
	set fileList [lreplace $fileList $activeWindow $activeWindow]

	# remove widgets from geometry manager

	grid forget $text
	grid forget $yScroll
	grid forget $xScroll

	# destroy widgets

	destroy $text
	destroy $yScroll
	destroy $xScroll
	unset undoArray($undo)
	unset redoArray($redo)
}

proc GetFile {} {

	# globalize

	global fileList
	global activeWindow

	# initalize and return variable

	set file [lindex $fileList $activeWindow]
	return $file
}

proc SetFile {file} {

	# globalize

	global fileList
	global activeWindow

	# set the file

	set fileList [lreplace $fileList $activeWindow $activeWindow $file]
	
	# take care of interface
	
	WindowsMenu
	FilebarList
}

proc GetText {} {

	# globalize

	global textList
	global activeWindow

	# initalize and return variable

	set text [lindex $textList $activeWindow]
	return $text
}

proc GetUndo {} {

	# globalize
	
	global undoList
	global activeWindow
	
	# initalize and return variable
	
	set undo [lindex $undoList $activeWindow]
	return $undo
}

proc GetRedo {} {

	# globalize
	
	global redoList
	global activeWindow
	
	# initalize and return variabe
	
	set redo [lindex $redoList $activeWindow]
	return $redo
}

proc WindowCount {} {

	# globalize

	global textList

	# return length

	set windows [llength $textList]
	return $windows
}

proc GetActive {} {

	# globalize

	global activeWindow

	# return active

	return $activeWindow
}

proc SetActive {active} {

	# globalize

	global activeWindow

	# set active

	set activeWindow $active
}

proc IncrementActive {} {

	# globalize

	global activeWindow

	# increment variable

	incr activeWindow 1
}

proc DecrementActive {} {

	# globaize

	global activeWindow

	# decrement variable

	incr activeWindow -1
}
