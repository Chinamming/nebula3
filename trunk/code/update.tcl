#--------------------------------------------------------------------
#   update.tcl
#   ==========
#--------------------------------------------------------------------

source ../bin/makemake.tcl
set binPrefix "..\\..\\bin\\win32"

#--------------------------------------------------------------------
# read any extension packages, and create list of targets
#--------------------------------------------------------------------
set global_gendeps 0
set targets {}
foreach ext [glob -nocomplain *.epk] {
    lappend targets [string range $ext [expr [string last / $ext]+1] end-4]
    source $ext
}

#--------------------------------------------------------------------
puts ""
puts ":: GENERATING..."
puts "================"

# vc8 project files

gen_solution8_win32 foundation_win32
gen_solution8_win32 render_win32
gen_solution8_win32 extlibs_win32
gen_solution8_win32 addons_win32
gen_solution8_win32 application_win32
gen_solution8_win32 tools_win32
gen_solution8_win32 tests_win32
gen_solution8_win32 benchmarks_win32
gen_solution8_win32 idlc

if {[file exists foundation_xbox360.epk]} {
    gen_solution_xbox360 foundation_xbox360
    gen_solution_xbox360 render_xbox360
    gen_solution_xbox360 extlibs_xbox360
    gen_solution_xbox360 addons_xbox360
    gen_solution_xbox360 application_xbox360
    gen_solution_xbox360 tools_xbox360
    gen_solution_xbox360 tests_xbox360
    gen_solution_xbox360 benchmarks_xbox360
}

if {[file exists foundation_wii.epk]} {
    gen_solutionxml_wii foundation_wii  
    gen_solutionxml_wii render_wii  
    gen_solutionxml_wii testviewer_wii
    gen_solutionxml_wii tools_wii
}

puts "done."

#--------------------------------------------------------------------
#   EOF
#--------------------------------------------------------------------

