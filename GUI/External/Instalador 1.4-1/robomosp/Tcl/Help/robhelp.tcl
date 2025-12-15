
set package_dir [file join [file dirname $argv0] ".."]

wm withdraw .
wm geometry . "+250+62"
wm overrideredirect . 1

lappend auto_path $package_dir
package require help 1.4

tkwait window .tophelpwindow
exit
