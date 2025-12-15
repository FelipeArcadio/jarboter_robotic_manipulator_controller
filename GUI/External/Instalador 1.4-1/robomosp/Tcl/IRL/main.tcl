# determine the platform

set platform $tcl_platform(platform)

# determine the directory

set directory [file dirname [info script]]
if {[string compare [file pathtype $directory] relative] == 0} then {
	set directory [file join [pwd] $directory]
}

# load the other files

source [file join $directory defaults.tcl]
source [file join $directory interface.tcl]
source [file join $directory commands.tcl]
source [file join $directory support.tcl]
source [file join $directory windows.tcl]
source [file join $directory dialogs.tcl]
source [file join $directory undo.tcl]
source [file join $directory syntax.tcl]
source [file join $directory ispell.tcl]

# initalize defaults

ReadDefaults
ReadSyntax

# initalize menus

StaticMenus
ProjectsMenu
CommandsMenu

# initalize toolbars

CreateToolbars
SetFilebar

# initalize things

SetUndo
SetComment
SetKeyword
SetIndent

# display application

New
ShowRoot

# if user specified command line args

if {$argc > 0} then {
	OpenProject $argv
}
