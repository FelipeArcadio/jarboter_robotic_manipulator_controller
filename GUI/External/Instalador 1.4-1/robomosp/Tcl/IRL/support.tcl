proc ReadData {file} {

	# open the file

	if {[catch {open $file r} stream]} then {
		tk_messageBox -type ok -icon error -message "Unable to read $file"
		return
	}

	# read and close stream

	set data [read $stream]
	close $stream
	return $data
}

proc WriteData {file data} {

	# open stream for selected file

	if {[catch {open $file w 0644} stream]} then {
		tk_messageBox -type ok -icon error -message "Unable to write $file"
		return
	}

	# write data and close stream

	puts -nonewline $stream $data
	flush $stream
	close $stream
}

proc OpenProject {projectList} {

	# loop through the project list

	foreach file $projectList {
	
		# if the file does not exist
		
		if {[file exists $file] == 0} then {
		
			# create window
			
			WriteData $file {}
			CreateWindow $file
			GridWindow
			update
			continue
		}
		
		# if the file contains no data
		
		if {[file size $file] == 0} then {
		
			# create window
			
			CreateWindow $file
			GridWindow
			update
			continue
		}
	
		# if the file does contain data
	
		set data [ReadData $file]
		CreateWindow $file
		set text [GetText]
		$text insert 1.0 $data
		$text mark set insert 1.0
		HighlightText 1.0 [$text index {end - 1 chars}]	
		GridWindow
		update
	}
}

proc CheckFile {} {

	# initalize data
	
	set text [GetText]
	set data [$text get 1.0 {end - 1 chars}]

	# initalize disk

	set file [GetFile]
	if {[file exists $file]} then {
		set disk [ReadData $file]
	} else {
		set disk {}
	}

	# deal with pesky new lines

	set data [string trimright $data]
	set disk [string trimright $disk]

	# return correctly

	if {[string compare $disk $data] == 0} then {
		return
	}
	return prompt
}

proc SavePrompt {} {

	# initalize variable
	
	set file [GetFile]

	# check to see if file exists and save it if user requests

	if {[file exists $file]} then {
		set choice [tk_messageBox -type yesnocancel -default yes -icon question -message "Save $file ?"]
		switch -exact $choice {
			yes {return save}
			no {return}
			cancel {return cancel}
		}
	} else {
		set choice [tk_messageBox -type yesnocancel -default yes -icon question -message {Save As ?}]
		switch -exact $choice {
			yes {return saveas}
			no {return}
			cancel {return cancel}
		}
	}
}

proc TextFind {startIndex stopIndex} {

	# initalize variable
	
	set text [GetText]
	
	# enter search loop
	
	while {1} {
	
		# call find string
	
		set return [FindString $startIndex $stopIndex]
		set findIndex [lindex $return 0]
		set findLength [lindex $return 1]
		
		# break if nothing found
	
		if {[string compare $findIndex {}] == 0} then {
			break
		}
		
		# update search start index
		
		set startIndex [$text index "$findIndex + $findLength chars"]
		
		# select and move to text

		$text tag remove sel 1.0 end
		$text tag add sel $findIndex "$findIndex + $findLength chars"
		$text mark set insert "$findIndex + $findLength chars"
		$text see insert
		
		# prompt user to stop searching
		
		set choice [tk_messageBox -type yesno -default yes -icon question -message {This instance ?}]
		
		# respond appropriately
		
		if {[string compare $choice yes] == 0} then {
			return
		}
	}
	
	# find string must not have been found
	
	tk_messageBox -type ok -icon info -message {Find string not found}
}

proc FilesFind {} {

	# loop through texts

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		
		# initalize variables
		
		set startIndex 1.0
		set stopIndex end
		set text [GetText]
		
		# enter search loop for current text
		
		while {1} {
		
			# call find string
		
			set return [FindString $startIndex $stopIndex]
			set findIndex [lindex $return 0]
			set findLength [lindex $return 1]
			
			# break if nothing found
		
			if {[string compare $findIndex {}] == 0} then {
				break
			}
			
			# update search start index
			
			set startIndex [$text index "$findIndex + $findLength chars"]
			
			# raise the current window
			
			RaiseWindow
			
			# select and move to text
	
			$text tag remove sel 1.0 end
			$text tag add sel $findIndex "$findIndex + $findLength chars"
			$text mark set insert "$findIndex + $findLength chars"
			$text see insert
			
			# prompt user to stop searching
			
			set choice [tk_messageBox -type yesno -default yes -icon question -message {This instance ?}]
			
			# respond appropriately
			
			if {[string compare $choice yes] == 0} then {
				return
			}
		}
	}
	SetActive $activeSave
	RaiseWindow
	
	# find string must not have been found
	
	tk_messageBox -type ok -icon info -message {Find string not found}
}

proc TextReplace {startIndex stopIndex} {

	# initalize variables
	
	set promptButton [GetDefault promptButton]
	set replaceString [GetDefault replaceString]
	set replaceLength [string length $replaceString]
	set replaceCount 0
	set text [GetText]
	
	# enter search loop
	
	while {1} {
	
		# call find string
	
		set return [FindString $startIndex $stopIndex]
		set findIndex [lindex $return 0]
		set findLength [lindex $return 1]
		
		# break if nothing found
	
		if {[string compare $findIndex {}] == 0} then {
			break
		}
		
		# update search start index
		
		set startIndex [$text index "$findIndex + $findLength chars"]
		
		# if user requested prompting
		
		if {$promptButton} then {
		
			# select and move to text
	
			$text tag remove sel 1.0 end
			$text tag add sel $findIndex "$findIndex + $findLength chars"
			$text mark set insert "$findIndex + $findLength chars"
			$text see insert
			
			# prompt user if the current instance is to be replace
			
			set choice [tk_messageBox -type yesnocancel -default yes -icon question -message {This instance ?}]
			
			# respond appropriately
			
			if {[string compare $choice cancel] == 0} then {
				return
			}
			if {[string compare $choice no] == 0} then {
				continue
			}
		}
		
		# delete text rembering undo

		set firstIndex $findIndex
		set lastIndex [$text index "$findIndex + $findLength chars"]
		set foundString [$text get $firstIndex $lastIndex]
		AppendUndo $text insert $findIndex $foundString
		$text delete $firstIndex $lastIndex
		
		# insert text rembering undo
		
		set firstIndex $findIndex
		$text insert $firstIndex $replaceString
		set lastIndex [$text index "$findIndex + $replaceLength chars"]
		AppendUndo $text delete $firstIndex $lastIndex
		
		# take care of syntax highlighting
		
		set firstIndex [$text index "$findIndex linestart"]
		set lastIndex [$text index "$findIndex lineend"]
		HighlightText $firstIndex $lastIndex
		
		# increment replace count
		
		incr replaceCount
	}
	
	# inform user of number or replacements
	
	tk_messageBox -type ok -icon info -message "$replaceCount replacements"
}

proc FilesReplace {} {

	# initalize variables

	set promptButton [GetDefault promptButton]
	set replaceString [GetDefault replaceString]
	set replaceLength [string length $replaceString]
	set replaceCount 0
	
	# loop through texts

	set activeSave [GetActive]
	for {SetActive 0} {[GetActive] < [WindowCount]} {IncrementActive} {
		
		# initalize variables
		
		set startIndex 1.0
		set stopIndex end
		set text [GetText]
		
		# enter search loop
		
		while {1} {
		
			# call find string
		
			set return [FindString $startIndex $stopIndex]
			set findIndex [lindex $return 0]
			set findLength [lindex $return 1]
			
			# break if nothing found
		
			if {[string compare $findIndex {}] == 0} then {
				break
			}
			
			# update search start index
			
			set startIndex [$text index "$findIndex + $findLength chars"]
			
			# if user requested prompting
			
			if {$promptButton} then {
			
				# raise the current window
				
				RaiseWindow
			
				# select and move to text
		
				$text tag remove sel 1.0 end
				$text tag add sel $findIndex "$findIndex + $findLength chars"
				$text mark set insert "$findIndex + $findLength chars"
				$text see insert
				
				# prompt user if the current instance is to be replace
				
				set choice [tk_messageBox -type yesnocancel -default yes -icon question -message {This instance ?}]
				
				# respond appropriately
				
				if {[string compare $choice cancel] == 0} then {
					return
				}
				if {[string compare $choice no] == 0} then {
					continue
				}
			}
			
			# delete text rembering undo
	
			set firstIndex $findIndex
			set lastIndex [$text index "$findIndex + $findLength chars"]
			set foundString [$text get $firstIndex $lastIndex]
			AppendUndo $text insert $findIndex $foundString
			$text delete $firstIndex $lastIndex
			
			# insert text rembering undo
			
			set firstIndex $findIndex
			$text insert $firstIndex $replaceString
			set lastIndex [$text index "$findIndex + $replaceLength chars"]
			AppendUndo $text delete $firstIndex $lastIndex
			
			# take care of syntax highlighting
			
			set firstIndex [$text index "$findIndex linestart"]
			set lastIndex [$text index "$findIndex lineend"]
			HighlightText $firstIndex $lastIndex
			
			# increment replace count
			
			incr replaceCount
		}
	}
	SetActive $activeSave
	RaiseWindow
	
	# inform user of number or replacements
	
	tk_messageBox -type ok -icon info -message "$replaceCount replacements"
}

proc FindString {startIndex stopIndex} {

	# initalize varibales
	
	set findString [GetDefault findString]
	set nocaseButton [GetDefault nocaseButton]
	set regexpButton [GetDefault regexpButton]
	set text [GetText]

	# construct search command

	set command [list $text search -count stringLength -- $findString $startIndex $stopIndex]
	if {$regexpButton} then {
		set command [linsert $command 4 -regexp]
	}
	if {$nocaseButton} then {
		set command [linsert $command 4 -nocase]
	}
	
	# search text

	set stringIndex {}
	set stringLength {}
	set stringIndex [eval $command]

	# return correctly

	if {[string compare $stringIndex {}] == 0} then {
		return
	}
	return [list $stringIndex $stringLength]
}

proc ExecCommand {command} {

	# execute command

	catch {eval "exec $command"} results
	if {[string compare $results {}] != 0} then {
		InfoDialog $results
	}
}
