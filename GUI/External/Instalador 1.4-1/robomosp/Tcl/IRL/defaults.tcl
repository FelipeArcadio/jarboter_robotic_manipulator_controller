# default functions for text editor

set defaultsArray() {}
unset defaultsArray()

proc ReadDefaults {} {

	# globalize

	global platform
	global directory
	global defaultsArray

	# determine which file to load

	if {[string compare $platform unix] != 0} then {
		set file [file join $directory tkscriberc.txt]
	} else {
		if {[file exists [file join ~ .tkscriberc]]} then {
			set file [file join ~ .tkscriberc]
		} else {
			set file [file join $directory tkscriberc.txt]
		}
	}

	# load data and initalize defaults array

	set data [ReadData $file]
	array set defaultsArray $data
}

proc SetDefault {option value} {

	# globalize

	global defaultsArray

	# initalize

	set defaultsArray($option) $value
}

proc GetDefault {option} {

	# globalize

	global defaultsArray

	# return value

	return $defaultsArray($option)
}

proc WriteDefaults {} {

	# globalize

	global platform
	global directory
	global defaultsArray

	# initalize file name

	if {[string compare $platform unix] != 0} then {
		set file [file join $directory tkscriberc.txt]
	} else {
		set file [file join ~ .tkscriberc]
	}

	# write defaults array

	set data [array get defaultsArray]
	WriteData $file $data
}
