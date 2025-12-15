proc StaticMenus {} {

	# create menubar

	menu .menubar -tearoff false

	# create file menu and add commands

	menu .menubar.file -tearoff false
	.menubar.file add command -label New -underline 0 -command New
	.menubar.file add command -label Open -underline 0 -command Open
	.menubar.file add separator
	.menubar.file add command -label Save -underline 0 -command Save
	.menubar.file add command -label {Save As} -underline 5 -command SaveAs
	.menubar.file add separator
	.menubar.file add command -label {Save All} -underline 7 -command SaveAll
	.menubar.file add command -label {Backup All} -underline 0 -command BackupAll
	.menubar.file add separator
	.menubar.file add command -label {Print} -underline 0 -command Print
	.menubar.file add command -label {Print Sel} -underline 4 -command PrintSel
	.menubar.file add separator
	.menubar.file add command -label Close -underline 0 -command Close
	.menubar.file add command -label Exit -underline 1 -command Exit

	# create projects menu and add commands

	menu .menubar.projects -tearoff false
	.menubar.projects add command -label Projects -underline 0 -command Projects
	.menubar.projects add separator

	# create edit menu and add commands

	menu .menubar.edit -tearoff false
	.menubar.edit add command -label Copy -underline 0 -command Copy
	.menubar.edit add command -label {Copy Append} -underline 5 -command CopyAppend
	.menubar.edit add separator
	.menubar.edit add command -label Cut -underline 2 -command Cut
	.menubar.edit add command -label {Cut Append} -underline 9 -command CutAppend
	.menubar.edit add separator
	.menubar.edit add command -label Delete -underline 0 -command Delete
	.menubar.edit add command -label Paste -underline 0 -command Paste
	.menubar.edit add separator
	.menubar.edit add command -label Undo -underline 0 -command Undo
	.menubar.edit add command -label Redo -underline 0 -command Redo
	.menubar.edit add separator
	.menubar.edit add command -label {Select All} -underline 0 -command SelectAll

	# create search menu and add commands

	menu .menubar.search -tearoff false
	.menubar.search add command -label Find -underline 0 -command Find
	.menubar.search add command -label {Big Find} -underline 0 -command BigFind
	.menubar.search add separator
	.menubar.search add command -label Replace -underline 0 -command Replace
	.menubar.search add command -label {Big Replace} -underline 2 -command BigReplace
	.menubar.search add separator
	.menubar.search add command -label {Goto Line} -underline 5 -command GotoLine
	.menubar.search add command -label {Goto Proc} -underline 5 -command GotoProc
	.menubar.search add separator
	.menubar.search add command -label {Find Next} -underline 5 -command FindNext

	# create window menu and add commands

	menu .menubar.texts -tearoff false
	.menubar.texts add command -label Previous -underline 0 -command Previous
	.menubar.texts add command -label Next -underline 0 -command Next
	.menubar.texts add separator
	
	# create marks window and add commands

	menu .menubar.marks -tearoff false
	.menubar.marks add command -label {Set Mark} -underline 4 -command SetMark
	.menubar.marks add command -label {Fix Menu} -underline 0 -command FixMenu
	.menubar.marks add separator
	
	# create selection menu and add commands

	menu .menubar.selection -tearoff false
	.menubar.selection add command -label Indent -underline 0 -command Indent
	.menubar.selection add command -label Unindent -underline 4 -command Unindent
	.menubar.selection add separator
	.menubar.selection add command -label Comment -underline 0 -command Comment
	.menubar.selection add command -label Uncomment -underline 8 -command Uncomment
	.menubar.selection add separator
	.menubar.selection add command -label Number -underline 0 -command Number
	.menubar.selection add command -label Unnumber -underline 5 -command Unnumber

	# create tools menu and add commands

	menu .menubar.tools -tearoff false
	.menubar.tools add command -label {List Files} -underline 0 -command ListFiles
	.menubar.tools add command -label {File Info} -underline 0 -command FileInfo
	.menubar.tools add command -label {Buffer Info} -underline 0 -command BufferInfo
	.menubar.tools add separator
	.menubar.tools add command -label {Highlight Fix} -underline 0 -command HighlightFix
	.menubar.tools add command -label {Undo/Redo Fix} -underline 0 -command UndoRedoFix
	.menubar.tools add command -label {Whitespace Fix} -underline 0 -command WhitespaceFix
	.menubar.tools add separator
	.menubar.tools add command -label {Color Picker} -underline 0 -command ColorPicker
	.menubar.tools add command -label {Spell Check} -underline 0 -command SpellCheck

	# create commands menu and add commands

	menu .menubar.commands -tearoff false
	.menubar.commands add command -label Commands -underline 0 -command Commands
	.menubar.commands add separator
	
	# create options menu and add commands

	menu .menubar.options -tearoff false
	.menubar.options add command -label {Set Fonts} -underline 4 -command SetFonts
	.menubar.options add command -label {Set Colors} -underline 4 -command SetColors
	.menubar.options add separator
	.menubar.options add command -label {Set Syntax} -underline 4 -command SetSyntax
	.menubar.options add command -label {Set General} -underline 4 -command SetGeneral
	.menubar.options add separator
	.menubar.options add command -label {Set Bindings} -underline 4 -command SetBindings
	.menubar.options add command -label {Set Buttons} -underline 6 -command SetButtons

	# add menus to menubar

	.menubar add cascade -label File -underline 0 -menu .menubar.file
	.menubar add cascade -label Projects -underline 0 -menu .menubar.projects
	.menubar add cascade -label Edit -underline 0 -menu .menubar.edit
	.menubar add cascade -label Search -underline 0 -menu .menubar.search
	.menubar add cascade -label Windows -underline 0 -menu .menubar.texts
	.menubar add cascade -label Marks -underline 0 -menu .menubar.marks
	.menubar add cascade -label Selection -underline 2 -menu .menubar.selection
	.menubar add cascade -label Tools -underline 0 -menu .menubar.tools
	.menubar add cascade -label Commands -underline 0 -menu .menubar.commands
	.menubar add cascade -label Options -underline 0 -menu .menubar.options
	. config -menu .menubar
}

proc ProjectsMenu {} {
	
	# initalize project names
	
	set projectNames [GetDefault projectNames]
	
	# delete any previous project enteries
	
	if {[.menubar.projects index 2] == 2} then {
		.menubar.projects delete 2 end
	}
	
	# add new project entries
	
	.menubar.projects add command -label "1 [lindex $projectNames 0]" -underline 0 -command FirstProject
	.menubar.projects add command -label "2 [lindex $projectNames 1]" -underline 0 -command SecondProject
	.menubar.projects add command -label "3 [lindex $projectNames 2]" -underline 0 -command ThirdProject
	.menubar.projects add command -label "4 [lindex $projectNames 3]" -underline 0 -command FourthProject
	.menubar.projects add command -label "5 [lindex $projectNames 4]" -underline 0 -command FifthProject
	.menubar.projects add command -label "6 [lindex $projectNames 5]" -underline 0 -command SixthProject
	.menubar.projects add command -label "7 [lindex $projectNames 6]" -underline 0 -command SeventhProject
	.menubar.projects add command -label "8 [lindex $projectNames 7]" -underline 0 -command EighthProject
}

proc CommandsMenu {} {

	# initalize command names
	
	set commandNames [GetDefault commandNames]
	
	# delete any previous command enteries
	
	if {[.menubar.commands index 2] == 2} then {
		.menubar.commands delete 2 end
	}
	
	# add new command enteries
	
	.menubar.commands add command -label "1 [lindex $commandNames 0]" -underline 0 -command FirstCommand
	.menubar.commands add command -label "2 [lindex $commandNames 1]" -underline 0 -command SecondCommand
	.menubar.commands add command -label "3 [lindex $commandNames 2]" -underline 0 -command ThirdCommand
	.menubar.commands add command -label "4 [lindex $commandNames 3]" -underline 0 -command FourthCommand
	.menubar.commands add command -label "5 [lindex $commandNames 4]" -underline 0 -command FifthCommand
	.menubar.commands add command -label "6 [lindex $commandNames 5]" -underline 0 -command SixthCommand
	.menubar.commands add command -label "7 [lindex $commandNames 6]" -underline 0 -command SeventhCommand
	.menubar.commands add command -label "8 [lindex $commandNames 7]" -underline 0 -command EighthCommand
}

proc WindowsMenu {} {

	# delete any previous window enteries
	
	if {[.menubar.texts index 3] == 3} then {
		.menubar.texts delete 3 end
	}
	
	# loop through all windows
	
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
	
		# initalize some variables
		
		set file [GetFile]
		set tail [file tail $file]
		set number [expr [GetActive] + 1]
		set window [GetActive]
		
		# add the menu entry
		
		.menubar.texts add command -label "$number $tail" -underline 0 -command "GotoWindow $window"
	}
	SetActive $activeSave
}

proc GotoWindow {window} {

	# switch to specified window
	
	SetActive $window
	RaiseWindow
}

proc MarksMenu {} {

	# delete any previous marks enteries
	
	if {[.menubar.marks index 3] == 3} then {
		.menubar.marks delete 3 end
	}
	
	# loop through all windows

	set number 1
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
	
		# initalize some variables
		
		set text [GetText]
		set marks [$text window names]
		
		# add the menu entry
		
		foreach widget $marks {
			regexp {[^\.]+$} $widget name
			.menubar.marks add command -label "$number $name" -underline 0 -command "GotoMark $widget"
			incr number 1
		}
	}
	SetActive $activeSave
}

proc GotoMark {widget} {

	# initalize text
	
	regexp {^.[^\.]+} $widget text
	set textList {}
	
	# initalize text list
	
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		lappend textList [GetText]
	}
	SetActive $activeSave

	# determine the window and return if closed

	set window [lsearch -exact $textList $text]
	if {$window < 0} then {
		MarksMenu
		tk_messageBox -type ok -icon info -message {Mark deleted}
		return
	}
	
	# determine the index and return if deleted
	
	if {[catch {$text index $widget} index]} then {
		MarksMenu
		tk_messageBox -type ok -icon info -message {Mark deleted}
		return
	}
	
	# switch to mark

	SetActive $window
	RaiseWindow
	$text mark set insert $index
	$text see insert
}

proc CreateToolbars {} {

	# create filebar

	label .filebar -font [GetDefault toolbarFont]
}

proc SetFilebar {} {

	# grid the filebar in its correct position

	if {[GetDefault filebarButton]} then {
		grid .filebar -row 4 -column 0 -columnspan 2 -sticky w
	} else {
		grid remove .filebar 
	}
}

proc FilebarList {} {

	# loop through windows

	set list {}
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {

		# get file tails

		set file [GetFile]
		set tail [file tail $file]

		# hightlight if active and append to list

		if {[GetActive] == $activeSave} then {
			set tail "<$tail>"
		} else {
			set tail " $tail "
		}

		# append file tail to file list

		set list "$list $tail"
	}
	SetActive $activeSave

	# config filebar widget

	.filebar config -text $list
}

proc ShowRoot {} {

	# configure resize behaviour

	grid rowconfigure . 1 -weight 1
	grid columnconfigure . 0 -weight 1

	# configure window manager

	wm title . {TK Scribe}
	wm iconname . {TK Scribe}

	# display window

	wm geometry . [GetDefault rootGeometry]
	update
}
