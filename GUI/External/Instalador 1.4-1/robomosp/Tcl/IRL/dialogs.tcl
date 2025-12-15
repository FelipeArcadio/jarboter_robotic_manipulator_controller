# dialog box functions for text editor

set dialogButton {}
set regexpButton {}
set nocaseButton {}
set boundsButton {}
set promptButton {}
set syntaxButton {}
set indentButton {}
set commentButton {}
set keywordButton {}
set wrapButton {}
set filebarButton {}
set undoButton {}
text .test

proc ProjectsDialog {} {

	# globalize

	global dialogButton

	# create dialog widgets

	set projectsToplevel [toplevel .projectsToplevel]
	set projectsFrame [frame $projectsToplevel.projectsFrame]
	set nameLabel [label $projectsFrame.nameLabel -text {Project Name}]
	set valueLabel [label $projectsFrame.valueLabel -text {Project Files}]
	set firstName [entry $projectsFrame.firstName -width 20]
	set firstEntry [entry $projectsFrame.firstEntry -width 40]
	set secondName [entry $projectsFrame.secondName -width 20]
	set secondEntry [entry $projectsFrame.secondEntry -width 40]
	set thirdName [entry $projectsFrame.thirdName -width 20]
	set thirdEntry [entry $projectsFrame.thirdEntry -width 40]
	set fourthName [entry $projectsFrame.fourthName -width 20]
	set fourthEntry [entry $projectsFrame.fourthEntry -width 40]
	set fifthName [entry $projectsFrame.fifthName -width 20]
	set fifthEntry [entry $projectsFrame.fifthEntry -width 40]
	set sixthName [entry $projectsFrame.sixthName -width 20]
	set sixthEntry [entry $projectsFrame.sixthEntry -width 40]
	set seventhName [entry $projectsFrame.seventhName -width 20]
	set seventhEntry [entry $projectsFrame.seventhEntry -width 40]
	set eighthName [entry $projectsFrame.eighthName -width 20]
	set eighthEntry [entry $projectsFrame.eighthEntry -width 40]
	set buttonsFrame [frame $projectsToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $projectsToplevel <KeyPress-Return> ReturnBind
	bind $projectsToplevel <KeyPress-Escape> EscapeBind
	
	# initalize widgets
	
	set projectNames [GetDefault projectNames]
	$firstName insert 0 [lindex $projectNames 0]
	$secondName insert 0 [lindex $projectNames 1]
	$thirdName insert 0 [lindex $projectNames 2]
	$fourthName insert 0 [lindex $projectNames 3]
	$fifthName insert 0 [lindex $projectNames 4]
	$sixthName insert 0 [lindex $projectNames 5]
	$seventhName insert 0 [lindex $projectNames 6]
	$eighthName insert 0 [lindex $projectNames 7]
	$firstEntry insert 0 [GetDefault firstProject]
	$secondEntry insert 0 [GetDefault secondProject]
	$thirdEntry insert 0 [GetDefault thirdProject]
	$fourthEntry insert 0 [GetDefault fourthProject]
	$fifthEntry insert 0 [GetDefault fifthProject]
	$sixthEntry insert 0 [GetDefault sixthProject]
	$seventhEntry insert 0 [GetDefault seventhProject]
	$eighthEntry insert 0 [GetDefault eighthProject]

	# grid all widgets

	grid $projectsFrame
	grid $nameLabel $valueLabel -padx 5 -pady 2 -sticky w
	grid $firstName $firstEntry  -padx 5 -pady 2 
	grid $secondName $secondEntry -padx 5 -pady 2 
	grid $thirdName $thirdEntry -padx 5 -pady 2 
	grid $fourthName $fourthEntry -padx 5 -pady 2
	grid $fifthName $fifthEntry -padx 5 -pady 2
	grid $sixthName $sixthEntry -padx 5 -pady 2
	grid $seventhName $seventhEntry -padx 5 -pady 2
	grid $eighthName $eighthEntry  -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $projectsToplevel [GetDefault projectsOffset]
	wm resizable $projectsToplevel false false
	$firstName selection range 0 end
	focus $firstName
	grab $projectsToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize lists
	
	set projectNames [list [$firstName get] [$secondName get] [$thirdName get] [$fourthName get] [$fifthName get] [$sixthName get] [$seventhName get] [$eighthName get]]
	set firstProject [$firstEntry get]
	set secondProject [$secondEntry get]
	set thirdProject [$thirdEntry get]
	set fourthProject [$fourthEntry get]
	set fifthProject [$fifthEntry get]
	set sixthProject [$sixthEntry get]
	set seventhProject [$seventhEntry get]
	set eighthProject [$eighthEntry get]

	# save geometry

	set projectsGeometry [wm geometry $projectsToplevel]
	regexp {\+.*} $projectsGeometry projectsOffset
	SetDefault projectsOffset $projectsOffset

	# restore focus and destroy dialog

	grab release $projectsToplevel
	set text [GetText]
	focus $text
	destroy $projectsToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $projectNames $firstProject $secondProject $thirdProject $fourthProject $fifthProject $sixthProject $seventhProject $eighthProject]
}

proc FindDialog {} {

	# gobalize

	global dialogButton
	global regexpButton
	global nocaseButton
	global boundsButton

	# create dialog widgets

	set findToplevel [toplevel .findToplevel]
	set findFrame [frame $findToplevel.findFrame]
	set optionsLabel [label $findFrame.optionsLabel -text {Find Options}]
	set regexpCheck [checkbutton $findFrame.regexpCheck -text {RegExp Match} -variable regexpButton]
	set nocaseCheck [checkbutton $findFrame.nocaseCheck -text {Ignore Case} -variable nocaseButton]
	set boundsLabel [label $findFrame.boundsLabel -text {Find Bounds}]
	set boundsFrame [frame $findFrame.boundsFrame]
	set selRadio [radiobutton $boundsFrame.selRadio -text sel -value sel -variable boundsButton]
	set textRadio [radiobutton $boundsFrame.textRadio -text text -value text -variable boundsButton]
	set filesRadio [radiobutton $boundsFrame.filesRadio -text files -value files -variable boundsButton]	
	set findLabel [label $findFrame.findLabel -text {Find String}]
	set findEntry [entry $findFrame.findEntry -width 40]
	set buttonsFrame [frame $findToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $findToplevel <KeyPress-Return> ReturnBind
	bind $findToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	set regexpButton [GetDefault regexpButton]
	set nocaseButton [GetDefault nocaseButton]
	set boundsButton [GetDefault boundsButton]
	$findEntry insert 0 [GetDefault findString]

	# grid all widgets

	grid $findFrame
	grid $optionsLabel -padx 5 -pady 2 -sticky w
	grid $regexpCheck -padx 5 -pady 2 -sticky w
	grid $nocaseCheck -padx 5 -pady 2 -sticky w
	grid $boundsLabel -padx 5 -pady 2 -sticky w
	grid $boundsFrame -padx 5 -pady 2 -sticky w
	grid $selRadio $textRadio $filesRadio
	grid $findLabel -padx 5 -pady 2 -sticky w
	grid $findEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $findToplevel [GetDefault findOffset]
	wm resizable $findToplevel false false
	$findEntry selection range 0 end
	focus $findEntry
	grab $findToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize local variables

	set findString [$findEntry get]

	# save geometry
	
	set findGeometry [wm geometry $findToplevel]
	regexp {\+.*} $findGeometry findOffset
	SetDefault findOffset $findOffset

	# restore focus and destroy dialog

	grab release $findToplevel
	set text [GetText]
	focus $text
	destroy $findToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $regexpButton $nocaseButton $boundsButton $findString]
}

proc BigFindDialog {} {

	# gobalize

	global dialogButton
	global regexpButton
	global nocaseButton
	global boundsButton

	# create dialog widgets

	set bigFindToplevel [toplevel .bigFindToplevel]
	set findFrame [frame $bigFindToplevel.findFrame]
	set optionsLabel [label $findFrame.optionsLabel -text {Find Options}]
	set regexpCheck [checkbutton $findFrame.regexpCheck -text {RegExp Match} -variable regexpButton]
	set nocaseCheck [checkbutton $findFrame.nocaseCheck -text {Ignore Case} -variable nocaseButton]
	set boundsLabel [label $findFrame.boundsLabel -text {Find Bounds}]
	set boundsFrame [frame $findFrame.boundsFrame]
	set selRadio [radiobutton $boundsFrame.selRadio -text sel -value sel -variable boundsButton]
	set textRadio [radiobutton $boundsFrame.textRadio -text text -value text -variable boundsButton]
	set filesRadio [radiobutton $boundsFrame.filesRadio -text files -value files -variable boundsButton]	
	set findLabel [label $findFrame.findLabel -text {Find String}]
	set findText [text $findFrame.findText -width 60 -height 6]
	set buttonsFrame [frame $bigFindToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $bigFindToplevel <KeyPress-Return> ReturnBind
	bind $bigFindToplevel <KeyPress-Escape> EscapeBind
	bindtags $findText [list $findText Text all]

	# initalize widgets

	set regexpButton [GetDefault regexpButton]
	set nocaseButton [GetDefault nocaseButton]
	set boundsButton [GetDefault boundsButton]
	$findText insert 1.0 [GetDefault findString]

	# grid all widgets

	grid $findFrame
	grid $optionsLabel -padx 5 -pady 2 -sticky w
	grid $regexpCheck -padx 5 -pady 2 -sticky w
	grid $nocaseCheck -padx 5 -pady 2 -sticky w
	grid $boundsLabel -padx 5 -pady 2 -sticky w
	grid $boundsFrame -padx 5 -pady 2 -sticky w
	grid $selRadio $textRadio $filesRadio
	grid $findLabel -padx 5 -pady 2 -sticky w
	grid $findText -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $bigFindToplevel [GetDefault bigFindOffset]
	wm resizable $bigFindToplevel false false
	$findText tag add sel 1.0 end
	focus $findText
	grab $bigFindToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize local variables

	set findString [$findText get 1.0 {end -1 chars}]

	# save geometry
	
	set bigFindGeometry [wm geometry $bigFindToplevel]
	regexp {\+.*} $bigFindGeometry bigFindOffset
	SetDefault bigFindOffset $bigFindOffset

	# restore focus and destroy dialog

	grab release $bigFindToplevel
	set text [GetText]
	focus $text
	destroy $bigFindToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $regexpButton $nocaseButton $boundsButton $findString]
}

proc ReplaceDialog {} {

	# gobalize

	global dialogButton
	global regexpButton
	global nocaseButton
	global promptButton
	global boundsButton

	# create dialog widgets

	set replaceToplevel [toplevel .replaceToplevel]
	set replaceFrame [frame $replaceToplevel.replaceFrame]
	set optionsLabel [label $replaceFrame.optionsLabel -text {Replace Options}]
	set regexpCheck [checkbutton $replaceFrame.regexpCheck -text {RegExp Match} -variable regexpButton]
	set nocaseCheck [checkbutton $replaceFrame.nocaseCheck -text {Ignore Case} -variable nocaseButton]
	set promptCheck [checkbutton $replaceFrame.promptCheck -text {Prompt Replace} -variable promptButton]
	set boundsLabel [label $replaceFrame.boundsLabel -text {Replace Bounds}]
	set boundsFrame [frame $replaceFrame.boundsFrame]
	set selRadio [radiobutton $boundsFrame.selRadio -text sel -value sel -variable boundsButton]
	set textRadio [radiobutton $boundsFrame.textRadio -text text -value text -variable boundsButton]
	set filesRadio [radiobutton $boundsFrame.filesRadio -text files -value files -variable boundsButton]	
	set findLabel [label $replaceFrame.findLabel -text {Find String}]
	set findEntry [entry $replaceFrame.findEntry -width 40]
	set replaceLabel [label $replaceFrame.replaceLabel -text {Replace String}]
	set replaceEntry [entry $replaceFrame.replaceEntry -width 40]
	set buttonsFrame [frame $replaceToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $replaceToplevel <KeyPress-Return> ReturnBind
	bind $replaceToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	set regexpButton [GetDefault regexpButton]
	set nocaseButton [GetDefault nocaseButton]
	set promptButton [GetDefault promptButton]
	set boundsButton [GetDefault boundsButton]
	$findEntry insert 0 [GetDefault findString]
	$replaceEntry insert 0 [GetDefault replaceString]

	# grid all widgets

	grid $replaceFrame
	grid $optionsLabel -padx 5 -pady 2 -sticky w
	grid $regexpCheck -padx 5 -pady 2 -sticky w
	grid $nocaseCheck -padx 5 -pady 2 -sticky w
	grid $promptCheck -padx 5 -pady 2 -sticky w
	grid $boundsLabel -padx 5 -pady 2 -sticky w
	grid $boundsFrame -padx 5 -pady 2 -sticky w
	grid $selRadio $textRadio $filesRadio
	grid $findLabel -padx 5 -pady 2 -sticky w
	grid $findEntry -padx 5 -pady 2
	grid $replaceLabel -padx 5 -pady 2 -sticky w
	grid $replaceEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $replaceToplevel [GetDefault replaceOffset]
	wm resizable $replaceToplevel false false
	$findEntry selection range 0 end
	focus $findEntry
	grab $replaceToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variables

	set findString [$findEntry get]
	set replaceString [$replaceEntry get]

	# save geometry

	set replaceGeometry [wm geometry $replaceToplevel]
	regexp {\+.*} $replaceGeometry replaceOffset
	SetDefault replaceOffset $replaceOffset

	# restore focus and destroy dialog

	grab release $replaceToplevel
	set text [GetText]
	focus $text
	destroy $replaceToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $regexpButton $nocaseButton $promptButton $boundsButton $findString $replaceString]
}

proc BigReplaceDialog {} {

	# gobalize

	global dialogButton
	global regexpButton
	global nocaseButton
	global promptButton
	global boundsButton

	# create dialog widgets

	set bigReplaceToplevel [toplevel .bigReplaceToplevel]
	set replaceFrame [frame $bigReplaceToplevel.replaceFrame]
	set optionsLabel [label $replaceFrame.optionsLabel -text {Replace Options}]
	set regexpCheck [checkbutton $replaceFrame.regexpCheck -text {RegExp Match} -variable regexpButton]
	set nocaseCheck [checkbutton $replaceFrame.nocaseCheck -text {Ignore Case} -variable nocaseButton]
	set promptCheck [checkbutton $replaceFrame.promptCheck -text {Prompt Replace} -variable promptButton]
	set boundsLabel [label $replaceFrame.boundsLabel -text {Replace Bounds}]
	set boundsFrame [frame $replaceFrame.boundsFrame]
	set selRadio [radiobutton $boundsFrame.selRadio -text sel -value sel -variable boundsButton]
	set textRadio [radiobutton $boundsFrame.textRadio -text text -value text -variable boundsButton]
	set filesRadio [radiobutton $boundsFrame.filesRadio -text files -value files -variable boundsButton]	
	set findLabel [label $replaceFrame.findLabel -text {Find String}]
	set findText [text $replaceFrame.findText -width 60 -height 6]
	set replaceLabel [label $replaceFrame.replaceLabel -text {Replace String}]
	set replaceText [text $replaceFrame.replaceText -width 60 -height 6]
	set buttonsFrame [frame $bigReplaceToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $bigReplaceToplevel <KeyPress-Return> ReturnBind
	bind $bigReplaceToplevel <KeyPress-Escape> EscapeBind
	bindtags $findText [list $findText Text all]
	bindtags $replaceText [list $replaceText Text all]

	# initalize widgets

	set regexpButton [GetDefault regexpButton]
	set nocaseButton [GetDefault nocaseButton]
	set promptButton [GetDefault promptButton]
	set boundsButton [GetDefault boundsButton]
	$findText insert 1.0 [GetDefault findString]
	$replaceText insert 1.0 [GetDefault replaceString]

	# grid all widgets

	grid $replaceFrame
	grid $optionsLabel -padx 5 -pady 2 -sticky w
	grid $regexpCheck -padx 5 -pady 2 -sticky w
	grid $nocaseCheck -padx 5 -pady 2 -sticky w
	grid $promptCheck -padx 5 -pady 2 -sticky w
	grid $boundsLabel -padx 5 -pady 2 -sticky w
	grid $boundsFrame -padx 5 -pady 2 -sticky w
	grid $selRadio $textRadio $filesRadio
	grid $findLabel -padx 5 -pady 2 -sticky w
	grid $findText -padx 5 -pady 2
	grid $replaceLabel -padx 5 -pady 2 -sticky w
	grid $replaceText -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $bigReplaceToplevel [GetDefault bigReplaceOffset]
	wm resizable $bigReplaceToplevel false false
	$findText tag add sel 1.0 end
	focus $findText
	grab $bigReplaceToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variables

	set findString [$findText get 1.0 {end - 1 chars}]
	set replaceString [$replaceText get 1.0 {end - 1 chars}]

	# save geometry

	set bigReplaceGeometry [wm geometry $bigReplaceToplevel]
	regexp {\+.*} $bigReplaceGeometry bigReplaceOffset
	SetDefault bigReplaceOffset $bigReplaceOffset

	# restore focus and destroy dialog

	grab release $bigReplaceToplevel
	set text [GetText]
	focus $text
	destroy $bigReplaceToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $regexpButton $nocaseButton $promptButton $boundsButton $findString $replaceString]
}

proc GotoLineDialog {} {

	# gobalize

	global dialogButton

	# create dialog widgets

	set lineToplevel [toplevel .lineToplevel]
	set lineFrame [frame $lineToplevel.lineFrame]
	set lineLabel [label $lineFrame.lineLabel -text {Line Number}]
	set lineEntry [entry $lineFrame.lineEntry -width 40]
	set buttonsFrame [frame $lineToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $lineToplevel <KeyPress-Return> ReturnBind
	bind $lineToplevel <KeyPress-Escape> EscapeBind

	# grid all widgets

	grid $lineFrame
	grid $lineLabel -padx 5 -pady 2 -sticky w
	grid $lineEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $lineToplevel [GetDefault lineOffset]
	wm resizable $lineToplevel false false
	$lineEntry selection range 0 end
	focus $lineEntry
	grab $lineToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variable

	set lineNumber [$lineEntry get]

	# save geometry
	
	set lineGeometry [wm geometry $lineToplevel]
	regexp {\+.*} $lineGeometry lineOffset
	SetDefault lineOffset $lineOffset

	# restore focus and destroy dialog

	grab release $lineToplevel
	set text [GetText]
	focus $text
	destroy $lineToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return $lineNumber
}

proc GotoProcDialog {} {

	# gobalize

	global procExprArray
	global dialogButton
	
	# initalize
	
	set procExpr $procExprArray([GetDefault syntaxButton])

	# create dialog widgets

	set procToplevel [toplevel .procToplevel]
	set procFrame [frame $procToplevel.procFrame]
	set procLabel [label $procFrame.procLabel -text {Proc Name}]
	set procListbox [listbox $procFrame.procListbox]
	set buttonsFrame [frame $procToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $procToplevel <KeyPress-Return> ReturnBind
	bind $procToplevel <KeyPress-Escape> EscapeBind

	# create procedure and line number lists

	set text [GetText]
	set procList {}
	set lineList {}

	# initalize start and stop index and line
	
	set firstIndex 1.0
	set lastIndex [$text index {end - 1 char}]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine
	
	for {set count $firstLine} {$count <= $lastLine} {incr count} {

		# initalize line indices
		
		set lineStart $count.0
		set lineStop [$text index "$count.0 lineend"]
		
		# search for the proc
		
		set procIndex [$text search -count procLength -regexp -- $procExpr $lineStart $lineStop]
		
		# check somthing found
		
		if {[string compare $procIndex {}] == 0} then {
			continue
		}
		
		# get procedure name
		
		set procStart $procIndex
		set procStop [$text index "$procIndex + $procLength chars"]
		set procString [$text get $procStart $procStop]
		set procLine $count
		
		# append to the lists
		
		lappend procList $procString
		lappend lineList $procLine
	}

	# initalize widget

	foreach proc $procList {
		$procListbox insert end $proc
	}

	# grid all widgets

	grid $procFrame
	grid $procLabel -padx 5 -pady 2 -sticky w
	grid $procListbox -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $procToplevel [GetDefault procOffset]
	wm resizable $procToplevel false false
	$procListbox activate 0
	$procListbox selection set 0
	focus $procListbox
	grab $procToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variable

	set element [$procListbox index active]
	set lineNumber [lindex $lineList $element]

	# save geometry
	
	set procGeometry [wm geometry $procToplevel]
	regexp {\+.*} $procGeometry procOffset
	SetDefault procOffset $procOffset

	# restore focus and destroy dialog

	grab release $procToplevel
	set text [GetText]
	focus $text
	destroy $procToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[llength $procList] == 0} then {
		return
	}
	return $lineNumber
}

proc SetMarkDialog {} {

	# gobalize

	global dialogButton

	# create dialog widgets

	set markToplevel [toplevel .markToplevel]
	set markFrame [frame $markToplevel.markFrame]
	set markLabel [label $markFrame.markLabel -text {Mark Name}]
	set markEntry [entry $markFrame.markEntry -width 40]
	set buttonsFrame [frame $markToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $markToplevel <KeyPress-Return> ReturnBind
	bind $markToplevel <KeyPress-Escape> EscapeBind

	# grid all widgets

	grid $markFrame
	grid $markLabel -padx 5 -pady 2 -sticky w
	grid $markEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $markToplevel [GetDefault markOffset]
	wm resizable $markToplevel false false
	$markEntry selection range 0 end
	focus $markEntry
	grab $markToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variable

	set name [$markEntry get]

	# save geometry
	
	set markGeometry [wm geometry $markToplevel]
	regexp {\+.*} $markGeometry markOffset
	SetDefault markOffset $markOffset

	# restore focus and destroy dialog

	grab release $markToplevel
	set text [GetText]
	focus $text
	destroy $markToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[string compare $name {}]  == 0} then {
		tk_messageBox -type ok -icon error -message {Mark name not valid}
		return
	}
	return $name
}

proc SetFontsDialog {} {

	# globalize

	global dialogButton

	# create dialog widgets

	set fontsToplevel [toplevel .fontsToplevel]
	set fontsFrame [frame $fontsToplevel.fontsFrame]
	set toolbarLabel [label $fontsFrame.toolbarLabel -text {Toolbar Font}]
	set toolbarEntry [entry $fontsFrame.toolbarEntry -width 40]
	set rootLabel [label $fontsFrame.rootLabel -text {Root Font}]
	set rootEntry [entry $fontsFrame.rootEntry -width 40]
	set markLabel [label $fontsFrame.markLabel -text {Mark Font}]
	set markEntry [entry $fontsFrame.markEntry -width 40]
	set commentLabel [label $fontsFrame.commentLabel -text {Comment Font}]
	set commentEntry [entry $fontsFrame.commentEntry -width 40]
	set keywordLabel [label $fontsFrame.keywordLabel -text {Keyword Font}]
	set keywordEntry [entry $fontsFrame.keywordEntry -width 40]
	set buttonsFrame [frame $fontsToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $fontsToplevel <KeyPress-Return> ReturnBind
	bind $fontsToplevel <KeyPress-Escape> EscapeBind
	
	# initalize widgets

	$toolbarEntry insert 0 [GetDefault toolbarFont]
	$rootEntry insert 0 [GetDefault rootFont]
	$markEntry insert 0 [GetDefault markFont]
	$commentEntry insert 0 [GetDefault commentFont]
	$keywordEntry insert 0 [GetDefault keywordFont]

	# grid all widgets

	grid $fontsFrame
	grid $toolbarLabel -padx 5 -pady 2 -sticky w
	grid $toolbarEntry -padx 5 -pady 2
	grid $rootLabel -padx 5 -pady 2 -sticky w
	grid $rootEntry -padx 5 -pady 2
	grid $markLabel -padx 5 -pady 2 -sticky w
	grid $markEntry -padx 5 -pady 2
	grid $commentLabel -padx 5 -pady 2 -sticky w
	grid $commentEntry -padx 5 -pady 2
	grid $keywordLabel -padx 5 -pady 2 -sticky w
	grid $keywordEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $fontsToplevel [GetDefault fontsOffset]
	wm resizable $fontsToplevel false false
	$toolbarEntry selection range 0 end
	focus $toolbarEntry
	grab $fontsToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize lists

	set toolbarFont [$toolbarEntry get]
	set rootFont [$rootEntry get]
	set markFont [$markEntry get]
	set commentFont [$commentEntry get]
	set keywordFont [$keywordEntry get]

	# save geometry
	
	set fontsGeometry [wm geometry $fontsToplevel]
	regexp {\+.*} $fontsGeometry fontsOffset
	SetDefault fontsOffset $fontsOffset

	# restore focus and destroy dialog

	grab release $fontsToplevel
	set text [GetText]
	focus $text
	destroy $fontsToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[catch {.test config -font $toolbarFont}]} then {
		tk_messageBox -type ok -icon error -message {Toolbar font not valid}
		return
	}
	if {[catch {.test config -font $rootFont}]} then {
		tk_messageBox -type ok -icon error -message {Root font not valid}
		return
	}
	if {[catch {.test config -font $markFont}]} then {
		tk_messageBox -type ok -icon error -message {Mark font not valid}
		return
	}
	if {[catch {.test config -font $commentFont}]} then {
		tk_messageBox -type ok -icon error -message {Comment font not valid}
		return
	}
	if {[catch {.test config -font $keywordFont}]} then {
		tk_messageBox -type ok -icon error -message {Keyword font not valid}
		return
	}
	return [list $toolbarFont $rootFont $markFont $commentFont $keywordFont]
}

proc SetColorsDialog {} {

	# globalize

	global dialogButton

	# create dialog widgets

	set colorsToplevel [toplevel .colorsToplevel]
	set colorsFrame [frame $colorsToplevel.colorsFrame]
	set backgroundLabel [label $colorsFrame.backgroundLabel -text {Background Color}]
	set backgroundEntry [entry $colorsFrame.backgroundEntry -width 40]
	set foregroundLabel [label $colorsFrame.foregroundLabel -text {Foreground Color}]
	set foregroundEntry [entry $colorsFrame.foregroundEntry -width 40]
	set markLabel [label $colorsFrame.markLabel -text {Mark Color}]
	set markEntry [entry $colorsFrame.markEntry -width 40]
	set commentLabel [label $colorsFrame.commentLabel -text {Comment Color}]
	set commentEntry [entry $colorsFrame.commentEntry -width 40]
	set keywordLabel [label $colorsFrame.keywordLabel -text {Keyword Color}]
	set keywordEntry [entry $colorsFrame.keywordEntry -width 40]
	set buttonsFrame [frame $colorsToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $colorsToplevel <KeyPress-Return> ReturnBind
	bind $colorsToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	$backgroundEntry insert 0 [GetDefault backgroundColor]
	$foregroundEntry insert 0 [GetDefault foregroundColor]
	$markEntry insert 0 [GetDefault markColor]
	$commentEntry insert 0 [GetDefault commentColor]
	$keywordEntry insert 0 [GetDefault keywordColor]

	# grid all widgets

	grid $colorsFrame
	grid $backgroundLabel -padx 5 -pady 2 -sticky w
	grid $backgroundEntry -padx 5 -pady 2
	grid $foregroundLabel -padx 5 -pady 2 -sticky w
	grid $foregroundEntry -padx 5 -pady 2
	grid $markLabel -padx 5 -pady 2 -sticky w
	grid $markEntry -padx 5 -pady 2
	grid $commentLabel -padx 5 -pady 2 -sticky w
	grid $commentEntry -padx 5 -pady 2
	grid $keywordLabel -padx 5 -pady 2 -sticky w
	grid $keywordEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $colorsToplevel [GetDefault colorsOffset]
	wm resizable $colorsToplevel false false
	$backgroundEntry selection range 0 end
	focus $backgroundEntry
	grab $colorsToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variables

	set backgroundColor [$backgroundEntry get]
	set foregroundColor [$foregroundEntry get]
	set markColor [$markEntry get]
	set commentColor [$commentEntry get]
	set keywordColor [$keywordEntry get]

	# save geometry
	
	set colorsGeometry [wm geometry $colorsToplevel]
	regexp {\+.*} $colorsGeometry colorsOffset
	SetDefault colorsOffset $colorsOffset

	# restore focus and destroy dialog

	grab release $colorsToplevel
	set text [GetText]
	focus $text
	destroy $colorsToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[catch {.test config -background $backgroundColor}]} then {
		tk_messageBox -type ok -icon error -message {Background color not vaild}
		return
	}
	if {[catch {.test config -background $foregroundColor}]} then {
		tk_messageBox -type ok -icon error -message {Foreground color not vaild}
		return
	}
	if {[catch {.test config -background $markColor}]} then {
		tk_messageBox -type ok -icon error -message {Mark color not vaild}
		return
	}
	if {[catch {.test config -background $commentColor}]} then {
		tk_messageBox -type ok -icon error -message {Comment color not vaild}
		return
	}
	if {[catch {.test config -background $keywordColor}]} then {
		tk_messageBox -type ok -icon error -message {Keyword color not vaild}
		return
	}
	return [list $backgroundColor $foregroundColor $markColor $commentColor $keywordColor]
}

proc SetSyntaxDialog {} {

	# globalize

	global syntaxList
	global dialogButton
	global syntaxButton
	global indentButton
	global commentButton
	global keywordButton
	global wrapButton
	
	# create dialog widgets
	
	set syntaxToplevel [toplevel .syntaxToplevel]
	set syntaxFrame [frame $syntaxToplevel.syntaxFrame]	
	set syntaxLabel [label $syntaxFrame.syntaxLabel -text {Syntax Highlighting}]
	set syntaxChoice [menubutton $syntaxFrame.syntaxChoice -relief raised -indicatoron true -width 12 -anchor w -textvariable syntaxButton]
	set indentCheck [checkbutton $syntaxFrame.indentCheck -text indent -variable indentButton]
	set commentCheck [checkbutton $syntaxFrame.commentCheck -text comments -variable commentButton]
	set keywordCheck [checkbutton $syntaxFrame.keywordCheck -text keywords -variable keywordButton]
	set wrapLabel [label $syntaxFrame.wrapLabel -text {Wrap Mode}]
	set wrapFrame [frame $syntaxFrame.wrapFrame]
	set noneRadio [radiobutton $wrapFrame.noneRadio -text none -value none -variable wrapButton]
	set charRadio [radiobutton $wrapFrame.charRadio -text char -value char -variable wrapButton]
	set wordRadio [radiobutton $wrapFrame.wordRadio -text word -value word -variable wrapButton]
	set tabsLabel [label $syntaxFrame.tabsLabel -text {Tab Stops}]
	set tabsEntry [entry $syntaxFrame.tabsEntry -width 40]
	set buttonsFrame [frame $syntaxToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $syntaxToplevel <KeyPress-Return> ReturnBind
	bind $syntaxToplevel <KeyPress-Escape> EscapeBind
	
	# create and attach syntax menu
	
	set syntaxMenu [menu $syntaxChoice.syntaxMenu -tearoff false]
	foreach language $syntaxList {
		$syntaxMenu add command -label $language -command "SetLanguage $language"
	}
	$syntaxChoice config -menu $syntaxMenu
	
	# initalize all widgets
	
	set indentButton [GetDefault indentButton]
	set syntaxButton [GetDefault syntaxButton]
	set commentButton [GetDefault commentButton]
	set keywordButton [GetDefault keywordButton]
	set wrapButton [GetDefault wrapButton]
	$tabsEntry insert 0 [GetDefault tabStops]

	# grid all widgets
		
	grid $syntaxFrame
	grid $syntaxLabel -padx 5 -pady 2 -sticky w
	grid $syntaxChoice -padx 5 -pady 2 -sticky w
	grid $indentCheck -padx 5 -pady 2 -sticky w
	grid $commentCheck -padx 5 -pady 2 -sticky w
	grid $keywordCheck -padx 5 -pady 2 -sticky w
	grid $wrapLabel -padx 5 -pady 2 -sticky w
	grid $wrapFrame -padx 5 -pady 2 -sticky w
	grid $noneRadio $charRadio $wordRadio
	grid $tabsLabel -padx 5 -pady 2 -sticky w
	grid $tabsEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2
	
	# display the dialog

	wm geometry $syntaxToplevel [GetDefault syntaxOffset]
	wm resizable $syntaxToplevel false false
	$tabsEntry selection range 0 end
	focus $tabsEntry
	grab $syntaxToplevel
	
	# wait for user interaction

	tkwait variable dialogButton

	# initalize variables

	set tabStops [$tabsEntry get]

	# save geometry
	
	set syntaxGeometry [wm geometry $syntaxToplevel]
	regexp {\+.*} $syntaxGeometry syntaxOffset
	SetDefault syntaxOffset $syntaxOffset
	
	# restore focus and destroy dialog

	grab release $syntaxToplevel
	set text [GetText]
	focus $text
	destroy $syntaxToplevel
	
	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[catch {.test config -tabs $tabStops}]} then {
		tk_messageBox -type ok -icon error -message {Tab stops not vaild}
		return
	}
	return [list $syntaxButton $indentButton $commentButton $keywordButton $wrapButton $tabStops]
}

proc SetLanguage {language} {

	# globalize
	
	global syntaxButton
	
	# initalize
	
	set syntaxButton $language
}

proc SetGeneralDialog {} {

	# globalize
	
	global filebarButton
	global undoButton
	global dialogButton

	# create dialog widgets

	set generalToplevel [toplevel .generalToplevel]
	set generalFrame [frame $generalToplevel.generalFrame]
	set generalLabel [label $generalFrame.generalLabel -text {General Options}]
	set filebarCheck [checkbutton $generalFrame.filebarCheck -text filebar -variable filebarButton]
	set undoCheck [checkbutton $generalFrame.undoCheck -text {undo/redo} -variable undoButton]
	set backupLabel [label $generalFrame.backupLabel -text {Backup Directory}]
	set backupEntry [entry $generalFrame.backupEntry -width 40]
	set tempLabel [label $generalFrame.tempLabel -text {Temporary Directory}]
	set tempEntry [entry $generalFrame.tempEntry -width 40]
	set helpLabel [label $generalFrame.helpLabel -text {Help Command}]
	set helpEntry [entry $generalFrame.helpEntry -width 40]
	set ispellLabel [label $generalFrame.ispellLabel -text {Ispell Command}]
	set ispellEntry [entry $generalFrame.ispellEntry -width 40]
	set enscriptLabel [label $generalFrame.enscriptLabel -text {Enscript Command}]
	set enscriptEntry [entry $generalFrame.enscriptEntry -width 40]
	set buttonsFrame [frame $generalToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $generalToplevel <KeyPress-Return> ReturnBind
	bind $generalToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	set filebarButton [GetDefault filebarButton]
	set undoButton [GetDefault undoButton]
	$backupEntry insert 0 [GetDefault backupDirectory]
	$tempEntry insert 0 [GetDefault tempDirectory]
	$helpEntry insert 0 [GetDefault helpCommand]
	$ispellEntry insert 0 [GetDefault ispellCommand]
	$enscriptEntry insert 0 [GetDefault enscriptCommand]

	# grid all widgets

	grid $generalFrame
	grid $generalLabel -padx 5 -pady 2 -sticky w
	grid $filebarCheck -padx 5 -pady 2 -sticky w
	grid $undoCheck -padx 5 -pady 2 -sticky w
	grid $backupLabel -padx 5 -pady 2 -sticky w
	grid $backupEntry -padx 5 -pady 2
	grid $tempLabel -padx 5 -pady 2 -sticky w
	grid $tempEntry -padx 5 -pady 2
	grid $helpLabel -padx 5 -pady 2 -sticky w
	grid $helpEntry -padx 5 -pady 2
	grid $ispellLabel -padx 5 -pady 2 -sticky w
	grid $ispellEntry -padx 5 -pady 2
	grid $enscriptLabel -padx 5 -pady 2 -sticky w
	grid $enscriptEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display the dialog

	wm geometry $generalToplevel [GetDefault generalOffset]
	wm resizable $generalToplevel false false
	$backupEntry selection range 0 end
	focus $backupEntry
	grab $generalToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize variables

	set backupDirectory [$backupEntry get]
	set tempDirectory [$tempEntry get]
	set helpCommand [$helpEntry get]
	set ispellCommand [$ispellEntry get]
	set enscriptCommand [$enscriptEntry get]

	# save geometry
	
	set generalGeometry [wm geometry $generalToplevel]
	regexp {\+.*} $generalGeometry generalOffset
	SetDefault generalOffset $generalOffset

	# restore focus and destroy dialog

	grab release $generalToplevel
	set text [GetText]
	focus $text
	destroy $generalToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	if {[file exists $backupDirectory] == 0} then {
		tk_messageBox -type ok -icon error -message {Backup directory not vaild}
		return
	}
	if {[file exists $tempDirectory] == 0} then {
		tk_messageBox -type ok -icon error -message {Temporary directory not valid}
		return
	}
	return [list $filebarButton $undoButton $backupDirectory $tempDirectory $helpCommand $ispellCommand $enscriptCommand]
}

proc BufferInfoDialog {} {

	# globalize

	global dialogButton

	# initalize text

	set file [GetFile]
	set text [GetText]

	# create dialog widgets

	set bufferToplevel [toplevel .bufferToplevel]
	set bufferFrame [frame $bufferToplevel.bufferFrame]
	set fileLabel [label $bufferFrame.fileLabel -text {File Name}]
	set fileValue [label $bufferFrame.fileValue]
	set linesLabel [label $bufferFrame.linesLabel -text {Line Count}]
	set linesValue [label $bufferFrame.linesValue]
	set charactersLabel [label $bufferFrame.charactersLabel -text {Character Count}]
	set charactersValue [label $bufferFrame.charactersValue]
	set insertLabel [label $bufferFrame.insertLabel -text {Insert Position}]
	set insertValue [label $bufferFrame.insertValue]
	set selLabel [label $bufferFrame.selLabel -text {Sel Delimits}]
	set selValue [label $bufferFrame.selValue]
	set buttonsFrame [frame $bufferToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $bufferToplevel <KeyPress-Return> ReturnBind
	bind $bufferToplevel <KeyPress-Escape> EscapeBind

	# calculate number of lines in buffer

	set textIndex [$text index end]
	regexp {^[0-9]+} $textIndex lineCount
	set lineTotal [expr $lineCount - 1]

	# calculate number of characters

	set characterCount 0
	set characterTotal 0
	for {set count 1} {$count < $lineCount} {incr count} {
		set textIndex [$text index "$count.0 lineend"]
		regexp {[0-9]+$} $textIndex characterCount
		incr characterTotal $characterCount
	}

	# retrive sel tag indicies

	if {[catch {$text index sel.first} selFirst]} then {
		set selFirst {}
	}
	if {[catch {$text index sel.last} selLast]} then {
		set selLast {}
	}

	# initalize widgets

	$fileValue config -text $file
	$linesValue config -text $lineTotal
	$charactersValue config -text $characterTotal
	$insertValue config -text [$text index insert]
	$selValue config -text "$selFirst $selLast"

	# grid all widgets

	grid $bufferFrame
	grid $fileLabel $fileValue -padx 5 -pady 2 -sticky w
	grid $linesLabel $linesValue -padx 5 -pady 2 -sticky w
	grid $charactersLabel $charactersValue -padx 5 -pady 2 -sticky w
	grid $insertLabel $insertValue -padx 5 -pady 2 -sticky w
	grid $selLabel $selValue -padx 5 -pady 2 -sticky w
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $bufferToplevel [GetDefault bufferOffset]
	wm resizable $bufferToplevel false false
	focus $cancelButton
	grab $bufferToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# save geometry
	
	set bufferGeometry [wm geometry $bufferToplevel]
	regexp {\+.*} $bufferGeometry bufferOffset
	SetDefault bufferOffset $bufferOffset

	# restore focus and destroy dialog

	grab release $bufferToplevel
	set text [GetText]
	focus $text
	destroy $bufferToplevel
}

proc FileInfoDialog {} {

	# globalize

	global dialogButton

	# check to see if file exists

	set file [GetFile]
	if {[file exists $file] == 0} then {
		tk_messageBox -type ok -icon error -message {File does not exist - use Save As}
		return
	}

	# create dialog widgets

	set fileToplevel [toplevel .fileToplevel]
	set fileFrame [frame $fileToplevel.fileFrame]
	set fileLabel [label $fileFrame.fileLabel -text {File Name}]
	set fileValue [label $fileFrame.fileValue]
	set typeLabel [label $fileFrame.typeLabel -text {File Type}]
	set typeValue [label $fileFrame.typeValue]
	set ownerLabel [label $fileFrame.ownerLabel -text {File Owner}]
	set ownerValue [label $fileFrame.ownerValue]
	set groupLabel [label $fileFrame.groupLabel -text {File Group}]
	set groupValue [label $fileFrame.groupValue]
	set permissionsLabel [label $fileFrame.permissionsLabel -text {File Permissions}]
	set permissionsValue [label $fileFrame.permissionsValue]
	set sizeLabel [label $fileFrame.sizeLabel -text {File Size}]
	set sizeValue [label $fileFrame.sizeValue]
	set accessedLabel [label $fileFrame.accessedLabel -text {Last Accessed}]
	set accessedValue [label $fileFrame.accessedValue]
	set modifiedLabel [label $fileFrame.modifiedLabel -text {Last Modified}]
	set modifiedValue [label $fileFrame.modifiedValue]
	set buttonsFrame [frame $fileToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $fileToplevel <KeyPress-Return> ReturnBind
	bind $fileToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	$fileValue config -text $file
	$typeValue config -text [file type $file]
	$ownerValue config -text [file attributes $file -owner]
	$groupValue config -text [file attributes $file -group]
	$permissionsValue config -text [file attributes $file -permissions]
	$sizeValue config -text [file size $file]
	$accessedValue config -text [clock format [file atime $file]]
	$modifiedValue config -text [clock format [file mtime $file]]

	# grid all widgets

	grid $fileFrame
	grid $fileLabel $fileValue -padx 5 -pady 2 -sticky w
	grid $typeLabel $typeValue -padx 5 -pady 2 -sticky w
	grid $ownerLabel $ownerValue -padx 5 -pady 2 -sticky w
	grid $groupLabel $groupValue -padx 5 -pady 2 -sticky w
	grid $permissionsLabel $permissionsValue -padx 5 -pady 2 -sticky w
	grid $sizeLabel $sizeValue -padx 5 -pady 2 -sticky w
	grid $accessedLabel $accessedValue -padx 5 -pady 2 -sticky w
	grid $modifiedLabel $modifiedValue -padx 5 -pady 2 -sticky w
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $fileToplevel [GetDefault fileOffset]
	wm resizable $fileToplevel false false
	focus $cancelButton
	grab $fileToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# save geometry
	
	set fileGeometry [wm geometry $fileToplevel]
	regexp {\+.*} $fileGeometry fileOffset
	SetDefault fileOffset $fileOffset

	# restore focus and destroy dialog

	grab release $fileToplevel
	set text [GetText]
	focus $text
	destroy $fileToplevel
}

proc CommandsDialog {} {

	# globalize

	global dialogButton

	# create dialog widgets

	set commandsToplevel [toplevel .commandsToplevel]
	set commandsFrame [frame $commandsToplevel.commandsFrame]
	set nameLabel [label $commandsFrame.nameLabel -text {Command Name}]
	set valueLabel [label $commandsFrame.valueLabel -text {Command Value}]
	set firstName [entry $commandsFrame.firstName -width 20]
	set firstEntry [entry $commandsFrame.firstEntry -width 40]
	set secondName [entry $commandsFrame.secondName -width 20]
	set secondEntry [entry $commandsFrame.secondEntry -width 40]
	set thirdName [entry $commandsFrame.thirdName -width 20]
	set thirdEntry [entry $commandsFrame.thirdEntry -width 40]
	set fourthName [entry $commandsFrame.fourthName -width 20]
	set fourthEntry [entry $commandsFrame.fourthEntry -width 40]
	set fifthName [entry $commandsFrame.fifthName -width 20]
	set fifthEntry [entry $commandsFrame.fifthEntry -width 40]
	set sixthName [entry $commandsFrame.sixthName -width 20]
	set sixthEntry [entry $commandsFrame.sixthEntry -width 40]
	set seventhName [entry $commandsFrame.seventhName -width 20]
	set seventhEntry [entry $commandsFrame.seventhEntry -width 40]
	set eighthName [entry $commandsFrame.eighthName -width 20]
	set eighthEntry [entry $commandsFrame.eighthEntry -width 40]
	set buttonsFrame [frame $commandsToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $commandsToplevel <KeyPress-Return> ReturnBind
	bind $commandsToplevel <KeyPress-Escape> EscapeBind

	# initalize widgets

	set commandNames [GetDefault commandNames]
	$firstName insert 0 [lindex $commandNames 0]
	$secondName insert 0 [lindex $commandNames 1]
	$thirdName insert 0 [lindex $commandNames 2]
	$fourthName insert 0 [lindex $commandNames 3]
	$fifthName insert 0 [lindex $commandNames 4]
	$sixthName insert 0 [lindex $commandNames 5]
	$seventhName insert 0 [lindex $commandNames 6]
	$eighthName insert 0 [lindex $commandNames 7]
	$firstEntry insert 0 [GetDefault firstCommand]
	$secondEntry insert 0 [GetDefault secondCommand]
	$thirdEntry insert 0 [GetDefault thirdCommand]
	$fourthEntry insert 0 [GetDefault fourthCommand]
	$fifthEntry insert 0 [GetDefault fifthCommand]
	$sixthEntry insert 0 [GetDefault sixthCommand]
	$seventhEntry insert 0 [GetDefault seventhCommand]
	$eighthEntry insert 0 [GetDefault eighthCommand]

	# grid all widgets

	grid $commandsFrame
	grid $nameLabel $valueLabel -padx 5 -pady 2 -sticky w
	grid $firstName $firstEntry -padx 5 -pady 2
	grid $secondName $secondEntry -padx 5 -pady 2
	grid $thirdName $thirdEntry -padx 5 -pady 2
	grid $fourthName $fourthEntry -padx 5 -pady 2
	grid $fifthName $fifthEntry -padx 5 -pady 2
	grid $sixthName $sixthEntry -padx 5 -pady 2
	grid $seventhName $seventhEntry -padx 5 -pady 2
	grid $eighthName $eighthEntry -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $commandsToplevel [GetDefault commandsOffset]
	wm resizable $commandsToplevel false false
	$firstName selection range 0 end
	focus $firstName
	grab $commandsToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# initalize lists

	set commandNames [list [$firstName get] [$secondName get] [$thirdName get] [$fourthName get] [$fifthName get] [$sixthName get] [$seventhName get] [$eighthName get]]
	set firstCommand [$firstEntry get]
	set secondCommand [$secondEntry get]
	set thirdCommand [$thirdEntry get]
	set fourthCommand [$fourthEntry get]
	set fifthCommand [$fifthEntry get]
	set sixthCommand [$sixthEntry get]
	set seventhCommand [$seventhEntry get]
	set eighthCommand [$eighthEntry get]
	
	# save geometry
	
	set commandsGeometry [wm geometry $commandsToplevel]
	regexp {\+.*} $commandsGeometry commandsOffset
	SetDefault commandsOffset $commandsOffset

	# restore focus and destroy dialog

	grab release $commandsToplevel
	set text [GetText]
	focus $text
	destroy $commandsToplevel

	# return correctly

	if {[string compare $dialogButton cancel] == 0} then {
		return
	}
	return [list $commandNames $firstCommand $secondCommand $thirdCommand $fourthCommand $fifthCommand $sixthCommand $seventhCommand $eighthCommand]
}

proc InfoDialog {string} {

	# globalize

	global dialogButton

	# create dialog widgets

	set infoToplevel [toplevel .infoToplevel]
	set infoFrame [frame $infoToplevel.infoFrame]
	set infoMessage [message $infoFrame.infoMessage -aspect 1000]
	set buttonsFrame [frame $infoToplevel.buttonsFrame]
	set cancelButton [button $buttonsFrame.cancelButton -text Cancel -command EscapeBind]
	set okButton [button $buttonsFrame.okButton -text OK -command ReturnBind]
	
	# initalize bindings
	
	bind $infoToplevel <KeyPress-Return> ReturnBind
	bind $infoToplevel <KeyPress-Escape> EscapeBind

	# initalize widget

	$infoMessage config -text $string

	# grid all widgets

	grid $infoFrame -sticky w
	grid $infoMessage -padx 5 -pady 2
	grid $buttonsFrame -sticky w
	grid $cancelButton $okButton -padx 5 -pady 2

	# display dialog

	wm geometry $infoToplevel [GetDefault infoOffset]
	wm resizable $infoToplevel false false
	focus $cancelButton
	grab $infoToplevel

	# wait for user interaction

	tkwait variable dialogButton

	# save geometry
	
	set infoGeometry [wm geometry $infoToplevel]
	regexp {\+.*} $infoGeometry infoOffset
	SetDefault infoOffset $infoOffset

	# restore focus and destroy dialog

	grab release $infoToplevel
	set text [GetText]
	focus $text
	destroy $infoToplevel
}

proc ReturnBind {} {

	# gloablize
	
	global dialogButton
	
	# initalize
	
	set dialogButton ok
}

proc EscapeBind {} {

	# globalize
	
	global dialogButton
	
	# initalize
	
	set dialogButton cancel
}
