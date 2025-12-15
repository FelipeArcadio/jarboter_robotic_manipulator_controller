# undo redo procedures

set isModifier 0
bind Text <ButtonPress-2> {}

proc SetUndo {} {

	# bindings for undo if undo enabled
	
	if {[GetDefault undoButton]} then {
	
		# bindings for modifiers
		
		bind all <KeyPress-Alt_L> {ModPress}
		bind all <KeyPress-Alt_R> {ModPress}
		bind all <KeyPress-Control_L> {ModPress}
		bind all <KeyPress-Control_R> {ModPress}
		bind all <KeyRelease-Alt_L> {ModRelease}
		bind all <KeyRelease-Alt_R> {ModRelease}
		bind all <KeyRelease-Control_L> {ModRelease}
		bind all <KeyRelease-Control_R> {ModRelease}
		
		# regular bindings
		
		bind undo <KeyPress> {InsertBind %W %A}
		bind undo <KeyPress-BackSpace> {BackSpaceBind %W}
		bind undo <KeyPress-Delete> {DeleteBind %W}
		bind undo <Control-KeyPress-d> {DBind %W}
		bind undo <Control-KeyPress-x> {XBind %W}
		bind undo <Control-KeyPress-o> {OBind %W}
		bind undo <Control-KeyPress-k> {KBind %W}
		
		# ensure menu entries are enabled
		
		.menubar.edit entryconfig Undo -state normal
		.menubar.edit entryconfig Redo -state normal
	} else {
		# bindings for modifiers
		
		bind all <KeyPress-Alt_L> {}
		bind all <KeyPress-Alt_R> {}
		bind all <KeyPress-Control_L> {}
		bind all <KeyPress-Control_R> {}
		bind all <KeyRelease-Alt_L> {}
		bind all <KeyRelease-Alt_R> {}
		bind all <KeyRelease-Control_L> {}
		bind all <KeyRelease-Control_R> {}
		
		# regular bindings
		
		bind undo <KeyPress> {}
		bind undo <KeyPress-BackSpace> {}
		bind undo <KeyPress-Delete> {}
		bind undo <Control-KeyPress-d> {}
		bind undo <Control-KeyPress-x> {}
		bind undo <Control-KeyPress-o> {}
		bind undo <Control-KeyPress-k> {}
		
		# ensure menu enteries are disabled
		
		.menubar.edit entryconfig Undo -state disabled
		.menubar.edit entryconfig Redo -state disabled
	}
	
	# loop through windows and clear undo and redo stacks

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		set undoStack [GetUndo]
		set redoStack [GetRedo]
		set undoArray($undoStack) {}
		set redoArray($redoStack) {}
	}
	SetActive $activeSave
}

proc ModPress {} {

	# globalize
	
	global isModifier

	# set modifier global
	
	set isModifier 1
}

proc ModRelease {} {

	# globalize
	
	global isModifier
	
	# set modifier global
	
	set isModifier 0
}

proc InsertBind {text char} {
	
	# globalize

	global isModifier
	
	# check to see modifier key is depressed
	
	if {$isModifier} then {
		return
	}
	
	# check to see if non printing character
	
	if {[string compare $char {}] == 0} then {
		return
	}
	
	# determine wether or not selection exists

	set return [$text tag nextrange sel 1.0 end]

	# respond accordingly
	
	if {[string compare $return {}] == 0} then {
	
		# initalize variable

		set index [$text index insert]	
		
		# add reverse operation to undo stack
		
		AppendUndo $text delete $index {}
	} else {
		
		# initalize variables
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		set string [$text get $firstIndex $lastIndex]
		
		# add reverse operation to undo stack
		
		AppendUndo $text insert $firstIndex $string
		AppendUndo $text delete $firstIndex {}
	}
}

proc BackSpaceBind {text} {

	# determine wether or not selection exists

	set return [$text tag nextrange sel 1.0 end]
	
	# respond accordingly
	
	if {[string compare $return {}] == 0} then {

		# check to see if insert mark is at start of text
		
		set index [$text index insert]
		if {[string compare $index 1.0] == 0} then {
			return
		}
	
		# initalize variables
		
		set index [$text index {insert - 1 chars}]
		set char [$text get $index]
		
		# add reverse operation to undo stack
		
		AppendUndo $text insert $index $char
	} else {
		
		# initalize variables
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		set string [$text get $firstIndex $lastIndex]
		
		# add reverse operation to undo stack

		AppendUndo $text insert $firstIndex $string
	}
}

proc DeleteBind {text} {

	# determine wether or not selection exists

	set return [$text tag nextrange sel 1.0 end]
	
	# respond accordingly
	
	if {[string compare $return {}] == 0} then {
	
		# initalize variables
	
		set index [$text index insert]
		set char [$text get $index]
		
		# add reverse operation to undo stack
		
		AppendUndo $text insert $index $char
	} else {
	
		# initalize variables
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		set string [$text get $firstIndex $lastIndex]
		
		# add reverse operation to undo stack

		AppendUndo $text insert $firstIndex $string
	}
}

proc DBind {text} {

	# control d deletes character to right of insert mark
	
	set index [$text index insert]
	set char [$text get $index]
	
	# add reverse operation to undo stack
	
	AppendUndo $text insert $index $char
}

proc XBind {text} {

	# control x deletes the selection if it exists
	
	set return [$text tag nextrange sel 1.0 end]
	
	# respond accordingly
	
	if {[string compare $return {}] != 0} then {
	
		# initalize variables
	
		set firstIndex [$text index sel.first]
		set lastIndex [$text index sel.last]
		set string [$text get $firstIndex $lastIndex]
		
		# add reverse operation to undo stack
		
		AppendUndo $text insert $firstIndex $string
	}
}

proc OBind {text} {

	# control o inserts newline
	
	set insertIndex [$text index insert]
	
	# add reverse operation to undo stack
	
	AppendUndo $text delete $insertIndex {}
}

proc KBind {text} {

	# control k deletes to end of line or deletes newline
	
	set firstIndex [$text index insert]
	set lastIndex [$text index {insert	lineend}]
	
	# check to see if end of line
	
	if {[string compare $firstIndex $lastIndex] == 0} then {
		
		# add reverse operation to undo stack
	
		AppendUndo $text insert $firstIndex \n
	} else {
	
		# initalize variable
		
		set selection [$text get $firstIndex $lastIndex]
		
		# add reverse operation to undo stack
		
		AppendUndo $text insert $firstIndex $selection
	}
}

proc AppendUndo {text action index detail} {

	# globalize
	
	global undoArray
	global redoArray
	
	# create list for undo operation and attach to correct undo list
	
	set operation [list $text $action $index $detail]
	set undoStack [GetUndo]
	lappend undoArray($undoStack) $operation
	
	# empty redo stack
	
	set redoStack [GetRedo]
	set redoArray($redoStack) {}
}

proc PushUndo {text action index detail} {

	# globalize
	
	global undoArray

	# create list for undo operation and attach to correct undo list

	set operation [list $text $action $index $detail]
	set undoStack [GetUndo]
	lappend undoArray($undoStack) $operation
}

proc PopUndo {} {

	# globalize
	
	global undoArray
	
	# get then delete last operation from undo stack
	
	set undoStack [GetUndo]
	set operation [lindex $undoArray($undoStack) end]
	set undoArray($undoStack) [lreplace $undoArray($undoStack) end end]
	
	# return correctly
	
	return $operation
}

proc PushRedo {text action index detail} {

	# globalize
	
	global redoArray
	
	# create list for redo operation and attach to correct redo stack
	
	set operation [list $text $action $index $detail]
	set redoStack [GetRedo]
	lappend redoArray($redoStack) $operation
}

proc PopRedo {} {

	# globalize
	
	global redoArray
	
	# get then delete last operation from undo stack
	
	set redoStack [GetRedo]
	set operation [lindex $redoArray($redoStack) end]
	set redoArray($redoStack) [lreplace $redoArray($redoStack) end end]
	
	# return correctly
	
	return $operation
}
