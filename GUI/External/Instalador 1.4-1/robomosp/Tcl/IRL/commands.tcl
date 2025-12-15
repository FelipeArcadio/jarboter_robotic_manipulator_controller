proc New {} {

	# create new window

	CreateWindow {Unsaved}
	GridWindow
}

proc Open {} {

	# get name of file to open

	set file [tk_getOpenFile]
	if {[string compare $file {}] == 0} then {
		return
	}
		
	# if the file does not exist
	
	if {[file exists $file] == 0} then {
	
		# create window
		
		WriteData $file {}
		CreateWindow $file
		GridWindow
		return
	}
	
	# if the file contains no data
	
	if {[file size $file] == 0} then {
	
		# create window
		
		CreateWindow $file
		GridWindow
		return
	}

	# if the file does contain data

	set data [ReadData $file]
	CreateWindow $file
	set text [GetText]
	$text insert 1.0 $data
	$text mark set insert 1.0
	HighlightText 1.0 [$text index {end - 1 chars}]
	GridWindow
}

proc Save {} {

	# globalize

	global undoArray
	global redoArray

	# initalize file

	set file [GetFile]
	if {[file exists $file] == 0} then {
		tk_messageBox -type ok -icon error -message {File does not exist}
		return
	}

	# initalize data and save file

	set text [GetText]
	set data [$text get 1.0 {end - 1 chars}]
	WriteData $file $data
	
	# empty undo and redo stacks
	
	set undoStack [GetUndo]
	set redoStack [GetRedo]
	set undoArray($undoStack) {}
	set redoArray($redoStack) {}
}

proc SaveAs {} {

	# globalize
	
	global undoArray
	global redoArray

	# get the save file

	set file [tk_getSaveFile]
	if {[string compare $file {}] == 0} then {
		return
	}

	# initalize data and save file

	set text [GetText]
	set data [$text get 1.0 {end - 1 chars}]
	WriteData $file $data
	SetFile $file
	
	# empty undo and redo stacks
	
	set undoStack [GetUndo]
	set redoStack [GetRedo]
	set undoArray($undoStack) {}
	set redoArray($redoStack) {}
}

proc SaveAll {} {	

	# globalize
	
	global undoArray
	global redoArray

	# loop through windows and save existing ones

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set file [GetFile]
		if {[file exists $file]} then {

			# initalize data and save file

			set text [GetText]
			set data [$text get 1.0 {end - 1 chars}]
			WriteData $file $data
		}
		
		# empty undo and redo stacks
		
		set undoStack [GetUndo]
		set redoStack [GetRedo]
		set undoArray($undoStack) {}
		set redoArray($redoStack) {}
	}
	SetActive $activeSave
}

proc BackupAll {} {

	# loop through all windows and backup existing ones

	set backupDirectory [GetDefault backupDirectory]
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set file [GetFile]
		if {[file exists $file]} then {

			# create backup file name

			set fileTail [file tail $file]
			set backupFile [file join $backupDirectory $fileTail]

			# initalize data and save file

			set text [GetText]
			set data [$text get 1.0 {end - 1 chars}]
			WriteData $backupFile $data
		}
	}
	SetActive $activeSave
}

proc Print {} {

	# globalize
	
	global platform
	
	# check platform
	
	if {[string compare $platform unix] != 0} then {
		tk_messageBox -type ok -icon info -message {Unix only feature}
		return
	}
	
	# initalize data
	
	set text [GetText]
	set data [$text get 1.0 {end - 1 chars}]

	# initalize enscript command

	set enscriptCommand [GetDefault enscriptCommand]
	set enscriptPipe [open $enscriptCommand r+]
	
	# pipe data to enscript 
	
	puts -nonewline $enscriptPipe $data
	flush $enscriptPipe
	
	# close pipe and display and return
	
	catch {close $enscriptPipe} results
	if {[string compare $results {}] != 0} then {
		InfoDialog $results
	}
}

proc PrintSel {} {

	# globalize
	
	global platform
	
	# check platform
	
	if {[string compare $platform unix] != 0} then {
		tk_messageBox -type ok -icon info -message {Unix only feature}
		return
	}

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and remove selection
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	$text tag remove sel 1.0 end

	# initalize data
	
	set text [GetText]
	set data [$text get $firstIndex $lastIndex]

	# initalize enscript command

	set enscriptCommand [GetDefault enscriptCommand]
	set enscriptPipe [open $enscriptCommand r+]
	
	# pipe data to enscript 
	
	puts -nonewline $enscriptPipe $data
	flush $enscriptPipe
	
	# close pipe and display and return
	
	catch {close $enscriptPipe} results
	if {[string compare $results {}] != 0} then {
		InfoDialog $results
	}
}

proc Close {} {

	# initalize file and data

	set activeSave [GetActive]

	# call save prompt if file is modified

	set return [CheckFile]
	if {[string compare $return prompt] == 0} then {
		set return [SavePrompt]
		switch -exact $return {
			save {Save}
			saveas {SaveAs}
			cancel {return}
		}
	}

	# deal with old window

	DestroyWindow

	# create window if none exist

	if {[WindowCount] == 0} then {
		New
		return
	}

	# display window

	if {$activeSave > 0} then {
		DecrementActive
	}
	RaiseWindow
	
	# updte the marks menu
	
	MarksMenu
}

proc Exit {} {

	# loop through windows and check if they shound be saved

	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {

		# call save prompt if file is modified

		set return [CheckFile]
		if {[string compare $return prompt] == 0} then {
			RaiseWindow
			set return [SavePrompt]
			switch -exact $return {
				save {Save}
				saveas {SaveAs}
				cancel {return}
			}
		}
	}

	# save geometry

	SetDefault rootGeometry =[wm geometry .]

	# write defaults and exit

	WriteDefaults
	exit
}

proc Projects {} {

	# get new project

	set projects [ProjectsDialog]
	if {[string compare $projects {}] == 0} then {
		return
	}

	# initalize variables

	set projectNames [lindex $projects 0]
	set firstProject [lindex $projects 1]
	set secondProject [lindex $projects 2]
	set thirdProject [lindex $projects 3]
	set fourthProject [lindex $projects 4]
	set fifthProject [lindex $projects 5]
	set sixthProject [lindex $projects 6]
	set seventhProject [lindex $projects 7]
	set eighthProject [lindex $projects 8]

	# update defaults array

	SetDefault projectNames $projectNames
	SetDefault firstProject $firstProject
	SetDefault secondProject $secondProject
	SetDefault thirdProject $thirdProject
	SetDefault fourthProject $fourthProject
	SetDefault fifthProject $fifthProject
	SetDefault sixthProject $sixthProject
	SetDefault seventhProject $seventhProject
	SetDefault eighthProject $eighthProject
	
	# update projects menu
	
	ProjectsMenu
}

proc FirstProject {} {

	# open first project

	set firstProject [GetDefault firstProject]
	OpenProject $firstProject
}

proc SecondProject {} {

	# open second project

	set secondProject [GetDefault secondProject]
	OpenProject $secondProject
}

proc ThirdProject {} {

	# open third project

	set thirdProject [GetDefault thirdProject]
	OpenProject $thirdProject
}

proc FourthProject {} {

	# open third project

	set fourthProject [GetDefault fourthProject]
	OpenProject $fourthProject
}

proc FifthProject {} {

	# open third project

	set fifthProject [GetDefault fifthProject]
	OpenProject $fifthProject
}

proc SixthProject {} {

	# open third project

	set sixthProject [GetDefault sixthProject]
	OpenProject $sixthProject
}

proc SeventhProject {} {

	# open third project

	set seventhProject [GetDefault seventhProject]
	OpenProject $seventhProject
}

proc EighthProject {} {

	# open third project

	set eighthProject [GetDefault eighthProject]
	OpenProject $eighthProject
}

proc Copy {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		return
	}
	
	# initalize first and last index 
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	
	# get selection and copy to clipboard
	
	set selection [$text get $firstIndex $lastIndex]
	clipboard clear
	clipboard append $selection
}

proc CopyAppend {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		return
	}
	
	# initalize first and last index 
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
		
	# get selection and append to clipboard
	
	set selection [$text get $firstIndex $lastIndex]
	clipboard append $selection
}

proc Cut {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		return
	}
	
	# initalize first and last index 
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	
	# add reverse operation to undo stack
	
	set selection [$text get $firstIndex $lastIndex]
	AppendUndo $text insert $firstIndex $selection
	
	# get selection delete selection and copy to clipboard
	
	$text delete $firstIndex $lastIndex
	clipboard clear
	clipboard append $selection
}

proc CutAppend {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		return
	}
	
	# initalize first and last index 
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	
	# add reverse operation to undo stack
	
	set selection [$text get $firstIndex $lastIndex]
	AppendUndo $text insert $firstIndex $selection
	
	# get selection delete selection and append to clipboard
	
	$text delete $firstIndex $lastIndex
	clipboard append $selection
}

proc Delete {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		return
	}
	
	# initalize first and last index 
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
		
	# add reverse operation to undo stack
	
	set selection [$text get $firstIndex $lastIndex]
	AppendUndo $text insert $firstIndex $selection
	
	# delete text
	
	$text delete $firstIndex $lastIndex
}

proc Paste {} {

	# get clipboard contents

	if {[catch {selection get -selection CLIPBOARD} contents]} then {
		return
	}

	# insert clipboard contents

	set text [GetText]
	set firstIndex [$text index insert]
	$text insert insert $contents
	set lastIndex [$text index insert]
	
	# add reverse operation to undo stack
	
	AppendUndo $text delete $firstIndex $lastIndex
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Undo {} {

	# retrive  operation and return if  stack is empty or corrupt
	
	set operation [PopUndo]
	if {[string compare $operation {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Undo stack empty}
		return
	}
	
	# initalize variables
	
	set text [lindex $operation 0]
	set action [lindex $operation 1]
	set index [lindex $operation 2]
	set detail [lindex $operation 3]
	
	# if delete operation add insert operation to redo stack
	
	if {[string compare $action delete] == 0} then {
	
		# may be character or string delete
		
		if {[string compare $detail {}] == 0} then {
			set char [$text get $index]
			PushRedo $text insert $index $char
		} else {
			set string [$text get $index $detail]
			PushRedo $text insert $index $string
		}
	}
	
	# peform the operation on the text
	
	if {[string compare $detail {}] == 0} then {
		set operation [lreplace $operation end end]
	}
	$text see $index
	eval $operation
	
	# if insert operation add delete operation to redo stack
	
	if {[string compare $action insert] == 0} then {
	
		# may be character or string insert
		
		set length [string length $detail]
		if {$length == 1} then {
			PushRedo $text delete $index {}
		} else {
			set firstIndex $index			
			set lastIndex [$text index "$index + $length chars"]
			PushRedo $text delete $firstIndex $lastIndex
		}
	}
	
	# syntax highlight
	
	set firstIndex [$text index "$index linestart"]
	set lastIndex [$text index "$index lineend"]
	HighlightText $firstIndex $lastIndex
}

proc Redo {} {

	# retrive  operation and return if  stack is empty or corrupt
	
	set operation [PopRedo]
	if {[string compare $operation {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Redo stack empty}
		return
	}
	
	# initalize variables
	
	set text [lindex $operation 0]
	set action [lindex $operation 1]
	set index [lindex $operation 2]
	set detail [lindex $operation 3]
		
	# if delete operation add insert operation to undo stack
	
	if {[string compare $action delete] == 0} then {
	
		# may be character or string delete
		
		if {[string compare $detail {}] == 0} then {
			set char [$text get $index]
			PushUndo $text insert $index $char
		} else {
			set string [$text get $index $detail]
			PushUndo $text insert $index $string
		}
	}
	
	# perform the operation on the text
	
	if {[string compare $detail {}] == 0} then {
		set operation [lreplace $operation end end]
	}
	$text see $index
	eval $operation
	
	# if insert operation add delete operation to undo stack
	
	if {[string compare $action insert] == 0} then {
	
		# may be character or string insert
		
		set length [string length $detail]
		if {$length == 1} then {
			PushUndo $text delete $index {}
		} else {
			set firstIndex $index
			set lastIndex [$text index "$index + $length chars"]
			PushUndo $text delete $firstIndex $lastIndex
		}
	}
			
	# syntax highlight
	
	set firstIndex [$text index "$index linestart"]
	set lastIndex [$text index "$index lineend"]
	HighlightText $firstIndex $lastIndex
}

proc SelectAll {} {

	# select all text

	set text [GetText]
	$text tag add sel 1.0 end
}

proc Find {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] != 0} then {
		
		# initalize first and last index 
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		$text tag remove sel 1.0 end
	}

	# call the find dialog

	set options [FindDialog]
	if {[string compare $options {}] == 0} then {
		return
	}

	# initalize variables

	set regexpButton [lindex $options 0]
	set nocaseButton [lindex $options 1]
	set boundsButton [lindex $options 2]
	set findString [lindex $options 3]

	# set defaults

	SetDefault regexpButton $regexpButton
	SetDefault nocaseButton $nocaseButton
	SetDefault boundsButton $boundsButton
	SetDefault findString $findString
	
	# check selection indices if find bounds is selection
	
	if {[string compare $boundsButton sel] == 0} then {
		if {[string compare $return {}] == 0} then {
			tk_messageBox -type ok -icon info -message {No selection}
			return
		}
	}
	
	# check find string
	
	if {[string compare $findString {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string null}
		return
	}

	# call correct find operation

	switch -exact $boundsButton {
		sel {TextFind $firstIndex $lastIndex}
		text {TextFind 1.0 end}
		files {FilesFind}
	}
}

proc BigFind {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] != 0} then {
		
		# initalize first and last index 
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		$text tag remove sel 1.0 end
	}

	# call the find dialog

	set options [BigFindDialog]
	if {[string compare $options {}] == 0} then {
		return
	}

	# initalize variables

	set regexpButton [lindex $options 0]
	set nocaseButton [lindex $options 1]
	set boundsButton [lindex $options 2]
	set findString [lindex $options 3]

	# set defaults

	SetDefault regexpButton $regexpButton
	SetDefault nocaseButton $nocaseButton
	SetDefault boundsButton $boundsButton
	SetDefault findString $findString
	
	# check selection indices if find bounds is selection
	
	if {[string compare $boundsButton sel] == 0} then {
		if {[string compare $return {}] == 0} then {
			tk_messageBox -type ok -icon info -message {No selection}
			return
		}
	}
	
	# check find string
	
	if {[string compare $findString {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string null}
		return
	}

	# call correct find operation

	switch -exact $boundsButton {
		sel {TextFind $firstIndex $lastIndex}
		text {TextFind 1.0 end}
		files {FilesFind}
	}
}

proc Replace {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] != 0} then {
		
		# initalize first and last index 
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		$text tag remove sel 1.0 end
	}

	# call the find dialog

	set options [ReplaceDialog]
	if {[string compare $options {}] == 0} then {
		return
	}

	# initalize variables

	set regexpButton [lindex $options 0]
	set nocaseButton [lindex $options 1]
	set promptButton [lindex $options 2]
	set boundsButton [lindex $options 3]
	set findString [lindex $options 4]
	set replaceString [lindex $options 5]

	# set defaults

	SetDefault regexpButton $regexpButton
	SetDefault nocaseButton $nocaseButton
	SetDefault promptButton $promptButton
	SetDefault boundsButton $boundsButton
	SetDefault findString $findString
	SetDefault replaceString $replaceString
	
	# check selection indices if find bounds is selection
	
	if {[string compare $boundsButton sel] == 0} then {
		if {[string compare $return {}] == 0} then {
			tk_messageBox -type ok -icon info -message {No selection}
			return
		}
	}
	
	# check find string
	
	if {[string compare $findString {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string null}
		return
	}

	# call correct find operation

	switch -exact $boundsButton {
		sel {TextReplace $firstIndex $lastIndex}
		text {TextReplace 1.0 end}
		files {FilesReplace}
	}
}

proc BigReplace {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] != 0} then {
		
		# initalize first and last index 
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		$text tag remove sel 1.0 end
	}
	
	# call the find dialog

	set options [BigReplaceDialog]
	if {[string compare $options {}] == 0} then {
		return
	}

	# initalize variables

	set regexpButton [lindex $options 0]
	set nocaseButton [lindex $options 1]
	set promptButton [lindex $options 2]
	set boundsButton [lindex $options 3]
	set findString [lindex $options 4]
	set replaceString [lindex $options 5]

	# set defaults

	SetDefault regexpButton $regexpButton
	SetDefault nocaseButton $nocaseButton
	SetDefault promptButton $promptButton
	SetDefault boundsButton $boundsButton
	SetDefault findString $findString
	SetDefault replaceString $replaceString
	
	# check selection indices if find bounds is selection
	
	if {[string compare $boundsButton sel] == 0} then {
		if {[string compare $return {}] == 0} then {
			tk_messageBox -type ok -icon info -message {No selection}
			return
		}
	}
	
	# check find string
	
	if {[string compare $findString {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string null}
		return
	}

	# call correct find operation

	switch -exact $boundsButton {
		sel {TextReplace $firstIndex $lastIndex}
		text {TextReplace 1.0 end}
		files {FilesReplace}
	}
}

proc GotoLine {} {

	# get line number

	set line [GotoLineDialog]
	if {[string compare $line {}] == 0} then {
		return
	}

	# move to selected line

	set text [GetText]
	if {[catch {$text mark set insert $line.0}]} then {
		return
	}
	catch {$text see insert}
}

proc GotoProc {} {

	# get line number

	set line [GotoProcDialog]
	if {[string compare $line {}] == 0} then {
		return
	}

	# move to the procedure

	set text [GetText]
	if {[catch {$text mark set insert $line.0}]} then {
		return
	}
	catch {$text see insert}
}

proc FindNext {} {

	# initalize variables
	
	set text [GetText]
	set findString [GetDefault findString]
	set startIndex [$text index insert]
	set stopIndex end
	
	# check find string
	
	if {[string compare $findString {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string null}
		return
	}

	# call find string

	set return [FindString $startIndex $stopIndex]
	set findIndex [lindex $return 0]
	set findLength [lindex $return 1]
	
	# return if nothing found

	if {[string compare $findIndex {}] == 0} then {
		tk_messageBox -type ok -icon info -message {Find string not found}
		return
	}
	
	# select and move to text

	$text tag remove sel 1.0 end
	$text tag add sel $findIndex "$findIndex + $findLength chars"
	$text mark set insert "$findIndex + $findLength chars"
	$text see insert
}

proc Previous {} {

	# return if already at first window

	if {[GetActive] == 0} then {
		return
	}

	# display previous window

	DecrementActive
	RaiseWindow
}

proc Next {} {

	# return if already at last window

	if {[GetActive] == [expr [WindowCount] - 1]} then {
		return
	}

	# display next window

	IncrementActive
	RaiseWindow
}

proc SetMark {} {

	# get the new mark

	set name [SetMarkDialog]
	if {[string compare $name {}] == 0} then {
		return
	}

	# create the mark

	set text [GetText]
	set index [$text index insert]
	set widget [label $text.$name -text $name -font [GetDefault markFont] -background [GetDefault markColor]]
	$text window create $index -align baseline -window $widget

	# update the menu
	
	MarksMenu
}

proc FixMenu {} {

	# update the marks menu

	MarksMenu
	
	# inform user of operation
	
	tk_messageBox -type ok -icon info -message {Marks menu refreshed}
}

proc Indent {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and insert numbers rembering the undo stack

	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# insert number
		
		set insertIndex "$count.0"
		$text insert $insertIndex \t
				
		# take care of undo
		
		AppendUndo $text delete $insertIndex {}
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Unindent {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and remove numbers rembering the undo stack

	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# search for tab
		
		set lineStart $count.0
		set lineStop [$text index "$count.0 lineend"]
		set deleteIndex [$text search -count deleteLength -- \t $lineStart $lineStop]
		
		# check somthing found
		
		if {[string compare $deleteIndex {}] == 0} then {
			continue
		}
		
		# take care of undo
		
		AppendUndo $text insert $deleteIndex \t
		
		# delete the characters
		
		$text delete $deleteIndex
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Comment {} {

	# globalize
	
	global commentSymbolArray
	
	# initalize comment symbol
	
	set comment $commentSymbolArray([GetDefault syntaxButton])

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and insert numbers rembering the undo stack

	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# insert number
		
		set insertIndex "$count.0"
		$text insert $insertIndex $comment
		set insertLength [string length $comment]
				
		# take care of undo
		
		set commentStart $insertIndex
		set commentStop [$text index "$insertIndex + $insertLength chars"]
		AppendUndo $text delete $commentStart $commentStop
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Uncomment {} {

	# globalize
	
	global commentSymbolArray
	
	# initalize comment symbol
	
	set comment $commentSymbolArray([GetDefault syntaxButton])

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and remove numbers rembering the undo stack

	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# search for number
		
		set lineStart $count.0
		set lineStop [$text index "$count.0 lineend"]
		set deleteIndex [$text search -count deleteLength -- $comment $lineStart $lineStop]
		
		# check somthing found
		
		if {[string compare $deleteIndex {}] == 0} then {
			continue
		}
		
		# take care of undo
		
		set commentStart $deleteIndex
		set commentStop [$text index "$deleteIndex + $deleteLength chars"]
		set comment [$text get $commentStart $commentStop]
		AppendUndo $text insert $commentStart $comment
		
		# delete the characters
		
		$text delete $commentStart $commentStop
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Number {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and insert numbers rembering the undo stack

	set number 1
	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# insert number
		
		set insertIndex "$count.0"
		$text insert $insertIndex "$number\t"
		set insertLength [expr [string length $number] + 1]
				
		# take care of undo
		
		set numberStart $insertIndex
		set numberStop [$text index "$insertIndex + $insertLength chars"]
		AppendUndo $text delete $numberStart $numberStop
		
		# increment number
		
		incr number 1
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc Unnumber {} {

	# check to see if selection exits
	
	set text [GetText]
	set return [$text tag nextrange sel 1.0 end]
	if {[string compare $return {}] == 0} then {
		tk_messageBox -type ok -icon info -message {No selection}
		return
	}
	
	# initalize first and last index and line
	
	set firstIndex [$text index sel.first]
	set lastIndex [$text index sel.last]
	regexp {^[0-9]+} $firstIndex firstLine
	regexp {^[0-9]+} $lastIndex lastLine

	# loop through lines and remove numbers rembering the undo stack

	for {set count $firstLine} {$count <= $lastLine} {incr count} {
	
		# search for number
		
		set lineStart $count.0
		set lineStop [$text index "$count.0 lineend"]
		set deleteIndex [$text search -count deleteLength -regexp -- {^[0-9]*\t} $lineStart $lineStop]
		
		# check somthing found
		
		if {[string compare $deleteIndex {}] == 0} then {
			continue
		}
		
		# take care of undo
		
		set numberStart $deleteIndex
		set numberStop [$text index "$deleteIndex + $deleteLength chars"]
		set number [$text get $numberStart $numberStop]
		AppendUndo $text insert $numberStart $number
		
		# delete the characters
		
		$text delete $numberStart $numberStop
	}
	
	# syntax highlight

	HighlightText $firstIndex $lastIndex
}

proc ListFiles {} {

	# loop through windows and make a list of filenames

	set list {}
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set file [GetFile]
		set list "$list $file \n"
	}
	SetActive $activeSave
	
	# display list of filenames

	InfoDialog $list
}

proc FileInfo {} {

	# just call the text info dialog

	FileInfoDialog
}

proc BufferInfo {} {

	# just call the buffer info dialog

	BufferInfoDialog
}

proc HighlightFix {} {

	# loop through the windows and re highlight

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set text [GetText]
		HighlightText 1.0 [$text index {end - 1 chars}]
	}
	SetActive $activeSave
	
	# inform user of operation
	
	tk_messageBox -type ok -icon info -message {Highlighting refreshed}
}

proc UndoRedoFix {} {

	# globalize
	
	global undoArray
	global redoArray

	# loop through windows and clear undo and redo stacks

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set undoStack [GetUndo]
		set redoStack [GetRedo]
		set undoArray($undoStack) {}
		set redoArray($redoStack) {}
	}
	SetActive $activeSave
	
	# inform user of operation
	
	tk_messageBox -type ok -icon info -message {Undo/Redo emptied}
}

proc WhitespaceFix {} {

	# loop through buffers
	
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
	
		# initalize variables
		
		set text [GetText]
		set firstIndex 1.0
		set lastIndex [$text index {end - 1 chars}]
		regexp {^[0-9]+} $firstIndex firstLine
		regexp {^[0-9]+} $lastIndex lastLine
		
		# loop through lines
		
		for {set count $firstLine} {$count <= $lastLine} {incr count} {
			
			# check for and delete non formatting tabs and spaces
			
			set spaceIndex {}
			set firstIndex $count.0
			set lastIndex [$text index "$count.0 lineend"]
			set spaceIndex [$text search -count spaceCount -regexp -- {[\t\s]+$} $firstIndex $lastIndex]
			
			# delete non formatting tabs and spaces if fount
			
			if {[string compare $spaceIndex {}] != 0} then {
				set firstIndex $spaceIndex
				set lastIndex [$text index "$spaceIndex + $spaceCount chars"]
				$text delete $firstIndex $lastIndex
			}
		}
	}
	SetActive $activeSave
	
	# inform user of operation
	
	tk_messageBox -type ok -icon info -message {Whitespace trimed}
}

proc ColorPicker {} {

	# just call inbuilt color dialog

	tk_chooseColor
}

proc SpellCheck {} {

	# promote next version
	
	tk_messageBox -type ok -icon info -message {Coming later}
}

proc Commands {} {	

	# get new commands

	set commands [CommandsDialog]
	if {[string compare $commands {}] == 0} then {
		return
	}

	# initalize variables

	set commandNames [lindex $commands 0]
	set firstCommand [lindex $commands 1]
	set secondCommand [lindex $commands 2]
	set thirdCommand [lindex $commands 3]
	set fourthCommand [lindex $commands 4]
	set fifthCommand [lindex $commands 5]
	set sixthCommand [lindex $commands 6]
	set seventhCommand [lindex $commands 7]
	set eighthCommand [lindex $commands 8]

	# update defaults array

	SetDefault commandNames $commandNames
	SetDefault firstCommand $firstCommand
	SetDefault secondCommand $secondCommand
	SetDefault thirdCommand $thirdCommand
	SetDefault fourthCommand $fourthCommand
	SetDefault fifthCommand $fifthCommand
	SetDefault sixthCommand $sixthCommand
	SetDefault seventhCommand $seventhCommand
	SetDefault eighthCommand $eighthCommand
	
	# take care of menu
	
	CommandsMenu
}

proc FirstCommand {} {

	# execute the command

	set firstCommand [GetDefault firstCommand]
	ExecCommand $firstCommand
}

proc SecondCommand {} {

	# execute the command

	set secondCommand [GetDefault secondCommand]
	ExecCommand $secondCommand
}

proc ThirdCommand {} {

	# execute the command

	set thirdCommand [GetDefault thirdCommand]
	ExecCommand $thirdCommand
}

proc FourthCommand {} {

	# execute the command

	set fourthCommand [GetDefault fourthCommand]
	ExecCommand $fourthCommand
}

proc FifthCommand {} {

	# execute the command

	set fifthCommand [GetDefault fifthCommand]
	ExecCommand $fifthCommand
}

proc SixthCommand {} {

	# execute the command

	set sixthCommand [GetDefault sixthCommand]
	ExecCommand $sixthCommand
}

proc SeventhCommand {} {

	# execute the command

	set seventhCommand [GetDefault seventhCommand]
	ExecCommand $seventhCommand
}

proc EighthCommand {} {

	# execute the command

	set eighthCommand [GetDefault eighthCommand]
	ExecCommand $eighthCommand
}

proc SetFonts {} {

	# get new fonts

	set fonts [SetFontsDialog]
	if {[string compare $fonts {}] == 0} then {
		return
	}

	# initalize variables

	set toolbarFont [lindex $fonts 0]
	set rootFont [lindex $fonts 1]
	set markFont [lindex $fonts 2]
	set commentFont [lindex $fonts 3]
	set keywordFont [lindex $fonts 4]

	# update defaults array

	SetDefault toolbarFont $toolbarFont
	SetDefault rootFont $rootFont
	SetDefault markFont $markFont
	SetDefault commentFont $commentFont
	SetDefault keywordFont $keywordFont

	# change existing text widgets

	.filebar config -font $toolbarFont
	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set text [GetText]
		$text config -font $rootFont
		set marks [$text window names]
		foreach widget $marks {
			$widget config -font $markFont
		}
		$text tag config comment -font $commentFont
		$text tag config keyword -font $keywordFont
	}
	SetActive $activeSave
}

proc SetColors {} {

	# get new fonts

	set colors [SetColorsDialog]
	if {[string compare $colors {}] == 0} then {
		return
	}

	# initalize variables

	set backgroundColor [lindex $colors 0]
	set foregroundColor [lindex $colors 1]
	set markColor [lindex $colors 2]
	set commentColor [lindex $colors 3]
	set keywordColor [lindex $colors 4]

	# update the defaults array

	SetDefault backgroundColor $backgroundColor
	SetDefault foregroundColor $foregroundColor
	SetDefault markColor $markColor
	SetDefault commentColor $commentColor
	SetDefault keywordColor $keywordColor

	# change existing text widgets

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set text [GetText]
		$text config -background $backgroundColor
		$text config -selectforeground $backgroundColor
		$text config -foreground $foregroundColor
		$text config -selectbackground $foregroundColor		
		$text config -insertbackground $foregroundColor
		set marks [$text window names]
		foreach widget $marks {
			$widget config -background $markColor
		}
		$text tag config comment -foreground $commentColor
		$text tag config keyword -foreground $keywordColor
	}
	SetActive $activeSave
}

proc SetSyntax {} {

	# call the syntax dialog
	
	set options [SetSyntaxDialog]
	if {[string compare $options {}] == 0} then {
		return
	}
	
	# initalize variables

	set syntaxButton [lindex $options 0]
	set indentButton [lindex $options 1]
	set commentButton [lindex $options 2]
	set keywordButton [lindex $options 3]
	set wrapButton [lindex $options 4]
	set tabStops [lindex $options 5]
	
	# update the defaults array

	SetDefault syntaxButton $syntaxButton
	SetDefault indentButton $indentButton
	SetDefault commentButton $commentButton
	SetDefault keywordButton $keywordButton
	SetDefault wrapButton $wrapButton
	SetDefault tabStops $tabStops
	
	# change existing text widgets

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set text [GetText]
		HighlightText 1.0 [$text index {end - 1 chars}]
		$text config -wrap $wrapButton
		$text config -tabs $tabStops
	}
	SetActive $activeSave
	
	# udate things
	
	SetIndent
	SetComment
	SetKeyword
}

proc SetGeneral {} {

	# call the general dialog

	set options [SetGeneralDialog]
	if {[string compare $options {}] == 0} then {
		return
	}

	# initalize variables

	set filebarButton [lindex $options 0]
	set undoButton [lindex $options 1]
	set backupDirectory [lindex $options 2]
	set tempDirectory [lindex $options 3]
	set helpCommand [lindex $options 5]
	set ispellCommand [lindex $options 5]
	set enscriptCommand [lindex $options 6]

	# update the defaults array

	SetDefault filebarButton $filebarButton
	SetDefault undoButton $undoButton
	SetDefault backupDirectory $backupDirectory
	SetDefault tempDirectory $tempDirectory
	SetDefault helpCommand $helpCommand
	SetDefault ispellCommand $ispellCommand
	SetDefault enscriptCommand $enscriptCommand
	
	# update things
	
	SetFilebar
	SetUndo
}

proc SetBindings {} {

	# promote next version
	
	tk_messageBox -type ok -icon info -message {Coming later}
}

proc SetButtons {} {

	# promote next version
	
	tk_messageBox -type ok -icon info -message {Coming later}
}
