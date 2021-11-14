#!/usr/bin/tclsh
if {[llength $argv] > 1} {
  set noav [llength $argv]
  for {set i 0} {$i < $noav} {incr i} {
    set rawfile$i [lindex $argv $i]
  }
  set filenameroot [file rootname $rawfile0]
  set fileextension [file extension $rawfile0]

} else {
  puts "\n\nsyntax: gnuplot1Daverage filenameA filenameB ... \n"
  puts "averages an arbitrary number of 1D gnuplot files, which do not have the same grid of x-data."
  puts "gnuplot files need to be linearly sampled from small x to big x values."
  puts "filenameA, filenameB, .. = filename of 1D XY in gnuplot format"
  exit 1
}

# PROC readrawfile name_of_raw_file
# Description:
proc readrawfile {name_of_raw_file} {
  if {[file exists $name_of_raw_file]} {
    if [catch {open $name_of_raw_file RDONLY} f] {
    } else {
      while {1} {
        gets $f zeile
        if [eof $f] break
        lappend field_of_ref $zeile
      }
      close $f
    }
  } else {
    puts "File $name_of_raw_file does not exist!"
    exit
  }
  return $field_of_ref
}

# PROC string2file (String, filename, Bool)
# Description:
# writes a string into a file. If file exists, will be overwritten. 
# file. It is possible to append the string to the filename by setting the 
# option to True
proc string2file {string nameoffile {boolappend 0}} {
  if {1!=$boolappend} {
    file delete -force  $nameoffile
  }
  set channel [open $nameoffile a+]
  puts  $channel $string
  eof   $channel
  close $channel
}

#read and analyze input files
  set maximumsmallestx -1e99
  set minimumbiggestx 1e99
  set incrxlist ""
  set xlists ""
  set ylists ""
  puts "1.) read files"
  for {set i 0} {$i < $noav} {incr i} {
    puts "read file [expr $i+1]"
    set xlist ""
    set ylist ""
    set raw$i [readrawfile [subst \$rawfile$i]]
    set numberoflines [llength [subst \$raw$i]]
    for {set j 0} {$j < $numberoflines} {incr j} {
      set line [split [regsub -all {\s+} [string trim [lindex [subst \$raw$i] $j]] { }]]
      if {[string index [lindex $line 0] 0] == "#"} {
        continue
      } elseif {[llength $line]!=0} {
        lappend xlist [lindex $line 0]
        lappend ylist [lindex $line 1]
      }
    }
    set smallestx [lindex $xlist 0]
    set biggestx  [lindex $xlist end]
    if {$smallestx > $maximumsmallestx} {
      set maximumsmallestx $smallestx
    }
    if {$biggestx < $minimumbiggestx} {
      set minimumbiggestx $biggestx
    }
    set incrx [expr {([lindex $xlist end]-[lindex $xlist 0])/([llength $xlist]-1)}]
    lappend incrxlist $incrx
    lappend xlists $xlist
    lappend ylists $ylist
  }
#make a common scale
#find maximum min and minimum max
#determine average x-increment 
set sum 0.0
foreach ix $incrxlist {
  set sum [expr {$sum+$ix}]
}
set deltax [expr {$sum/[llength $incrxlist]}]
#xscale
set minx [expr {$maximumsmallestx+$deltax}]
set maxx [expr {$minx+double(floor(($minimumbiggestx-$maximumsmallestx)/$deltax)-4)*$deltax}]
set nopoints [expr {int(floor(($minimumbiggestx-$maximumsmallestx)/$deltax))-3}]

puts "2.) common x-range"
puts "from: $maximumsmallestx"
puts "to: $minimumbiggestx"

#add data using linear interpolation onto new scale
set xlistav ""
set ylistav [lrepeat $nopoints 0.0]
puts "3.) average interpolated datasets"
set x $minx
foreach y $ylistav {
  lappend xlistav $x
  set x [expr {$x+$deltax}]
}
set xioffset [lrepeat $noav 0]
for {set i 0} {$i < $nopoints} {incr i} {
  set x [lindex $xlistav $i]
  set y [lindex $ylistav $i]
  for {set j 0} {$j < $noav} {incr j} {
    while {$x > [lindex [lindex $xlists $j] [expr {$i+[lindex $xioffset $j]}]]} {
      lset xioffset $j [expr {1+[lindex $xioffset $j]}]
    }
    set x0 [lindex [lindex $xlists $j] [expr {$i+[lindex $xioffset $j]}]]
    set x1 [lindex [lindex $xlists $j] [expr {-1+$i+[lindex $xioffset $j]}]]
    set y0 [lindex [lindex $ylists $j] [expr {$i+[lindex $xioffset $j]}]]
    set y1 [lindex [lindex $ylists $j] [expr {-1+$i+[lindex $xioffset $j]}]]
    lset ylistav $i [expr {($y0*($x1-$x)+$y1*($x-$x0))/($x1-$x0)+[lindex $ylistav $i]}]
  }
}
puts "4.) save result"
#write result 
#name: first filename appended with an "_av"
set returnstring ""
foreach x $xlistav y $ylistav {
  append returnstring "$x [expr {$y/$noav}]\n"
}

set filename "$filenameroot\_av$fileextension"
string2file $returnstring $filename
puts "The averaged file was saved to a file named \n\"$filename\""
