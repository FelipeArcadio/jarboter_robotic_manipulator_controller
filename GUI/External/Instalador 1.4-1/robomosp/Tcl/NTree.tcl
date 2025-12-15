# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : NTree.tcl
#  Date         : 2001-11-09
#  Description  : Implementation of n-ary trees
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  NTree.tcl
#		This file implements the console window.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------

namespace eval NTree {

  # Procedures :
  # proc SetFather { NTree Father }
  # proc GetFather { NTree }
  # proc SetFirstSon { NTree FirstSon }
  # proc GetFirstSon { NTree }
  # proc SetLastSon { NTree LastSon }
  # proc GetLastSon { NTree }
  # proc SetNextBrother { NTree NextBrother }
  # proc GetNextBrother { NTree }
  # proc SetPreviousBrother { NTree PreviousBrother }
  # proc GetPreviousBrother { NTree }
  # proc SetNumberSons { NTree NumberSons }
  # proc GetNumberSons { NTree }
  # proc SetData { NTree Data }
  # proc GetData { NTree }
  # proc Set { { Data {} } }
  # proc Empty { NTree }
  # proc Add { NTree Son }
  # proc Insert { NTree Son }
  # proc DisAssociate { NTree }
  # proc Destroy { NTree }
  # proc Print { NTree { Level 0 } }
  # proc Weight { NTree }
  # proc Root { NTree }
  # proc SonIndex { NTree }

  # Initialization of global variables
  variable Null {}
  # Node Id, it is incremented anytime the TkNTree::Set procedure is invoqued
  variable Id 1

  # SetFather  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetFather { NTree Father } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 0 0 $Father ]
  }

  # GetFather  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetFather { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 0 ]
  }

  # SetFirstSon  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetFirstSon { NTree FirstSon } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 1 1 $FirstSon ]
  }

  # GetFirstSon  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetFirstSon { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 1 ]
  }

  # SetLastSon  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetLastSon { NTree LastSon } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 2 2 $LastSon ]
  }

  # GetLastSon  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetLastSon { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 2 ]
  }

  # SetNextBrother  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetNextBrother { NTree NextBrother } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 3 3 $NextBrother ]
  }

  # GetNextBrother  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextBrother { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 3 ]
  }

  # SetPreviousBrother  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPreviousBrother { NTree PreviousBrother } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 4 4 $PreviousBrother ]
  }

  # GetPreviousBrother  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPreviousBrother { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 4 ]
  }

  # SetNumberSons  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetNumberSons { NTree NumberSons } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 5 5 $NumberSons ]
  }

  # GetNumberSons  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNumberSons { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 5 ]
  }

  # SetData  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetData { NTree Data } {
    variable $NTree
    set $NTree [ lreplace [ expr $$NTree ] 6 6 $Data ]
  }

  # GetData  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetData { NTree } {
    variable $NTree
    return [ lindex [ expr $$NTree ] 6 ]
  }

  # Set  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Set { { Data {} } } {
    # <0.Father> <1.FirstSon> <2.LastSon> <3.NextBrother> <4.PreviousBrother> <5.NumberSons> <6.Data>
    # Father:          Id to the father.
    # FirstSon:        Id to the first son.
    # LastSon:         Id to the last son.
    # NextBrother:     Id to the next brother.
    # PreviousBrother: Id to the previous brother.
    # NumberSons:      It stores the Node's number of sons.
    # Data:            It stores the Node's data. For this application, the TkNTree
    #                  component data is stored.
    variable Null
    variable Id
    variable Node$Id [ list $Null $Null $Null $Null $Null 0 $Data ]
    incr Id
    return Node[ expr $Id - 1 ]
  }

  # Empty  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Empty { NTree } {
    return [ expr [ llength $NTree ] == 0 ]
  }

  # Add  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Add { NTree Son } {
    SetFather $Son $NTree
    if { [ Empty [ GetFirstSon $NTree ] ] } {
      SetFirstSon $NTree $Son
      SetLastSon $NTree $Son
    } else {
      SetNextBrother [ GetLastSon $NTree ] $Son
      SetPreviousBrother $Son [ GetLastSon $NTree ]
      SetLastSon $NTree $Son
    }
    SetNumberSons $NTree [ expr [ GetNumberSons $NTree ] + 1 ]
  }

  # Insert  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Insert { NTree Son } {
    SetFather $Son $NTree
    if { [ Empty [ GetFirstSon $NTree ] ] } {
      SetFirstSon $NTree $Son
      SetLastSon $NTree $Son
    } else {
      SetPreviousBrother $Son [ GetPreviousBrother [GetLastSon $NTree] ]
      SetNextBrother $Son [ GetLastSon $NTree ]
      SetNextBrother [ GetPreviousBrother [ GetLastSon $NTree] ] $Son
      SetPreviousBrother [ GetLastSon $NTree ] $Son
    }
    SetNumberSons $NTree [ expr [ GetNumberSons $NTree ] + 1 ]
  }

  # DisAssociate  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DisAssociate { NTree } {
    variable Null
    if { ! [ Empty [ GetFather $NTree ] ] } {
      if { [ Empty [ GetPreviousBrother $NTree ] ] } {
        if { [ Empty [ GetNextBrother $NTree ] ] } {
          SetFirstSon [ GetFather $NTree ] $Null
          SetLastSon [ GetFather $NTree ] $Null
        } else {
          SetFirstSon [ GetFather $NTree ] [ GetNextBrother $NTree ]
          SetPreviousBrother [ GetNextBrother $NTree ] $Null
        }
      } else {
        if { [ Empty [ GetNextBrother $NTree ] ] } {
          SetLastSon [ GetFather $NTree ] [ GetPreviousBrother $NTree ]
          SetNextBrother [ GetPreviousBrother $NTree ] $Null
        } else {
          SetPreviousBrother [ GetNextBrother $NTree ] [ GetPreviousBrother $NTree ]
          SetNextBrother [ GetPreviousBrother $NTree ] [ GetNextBrother $NTree ]
        }
      }
    }
    SetNumberSons [ GetFather $NTree ] [ expr [ GetNumberSons [ GetFather $NTree ] ] - 1 ]
    SetPreviousBrother $NTree $Null
    SetNextBrother $NTree $Null
    SetFather $NTree $Null
  }

  # Destroy  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Destroy { NTree } {
    variable $NTree
    if { ! [ Empty $NTree ] } {
      set TemporaryNTreeA [ GetFirstSon $NTree ]
      while { ! [ Empty $TemporaryNTreeA ] } {
        set TemporaryNTreeB [ GetNextBrother $TemporaryNTreeA ]
        Destroy $TemporaryNTreeA
        set TemporaryNTreeA $TemporaryNTreeB
      }
      unset $NTree
    }
  } 

  # Print  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Print { NTree { Level 0 } } {
    if { ! [ Empty $NTree ] } {
      set i 0
      while { $i < $Level } {
        puts -nonewline "\ \ \ \ "
        incr i
      }
      puts "[ GetData $NTree ]"
      set TemporaryNTree [ GetFirstSon $NTree ]
      while { ! [ Empty $TemporaryNTree ] } {
        Print $TemporaryNTree [ expr $Level + 1 ]
        set TemporaryNTree [ GetNextBrother $TemporaryNTree ]
      }
    }
  }

  # Weight  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Weight { NTree } {
    if { ! [ Empty $NTree ] } {
      set vWeight 0
      set TemporaryNTree [ GetFirstSon $NTree ]
      while { ! [ Empty $TemporaryNTree ] } {
        set vWeight [ expr $vWeight + [ Weight $TemporaryNTree ] ]
        set TemporaryNTree [ GetNextBrother $TemporaryNTree ]
      }
      return [ expr $vWeight + 1 ]
    } else {
      return 0
    }
  }

  # Root  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Root { NTree } {
    if { ! [ Empty $NTree ] } {
      if { ! [ Empty [ GetFather $NTree ] ] } {
        return [ Root [ GetFather $NTree ] ]
      } else {
        return $NTree
      }
    }
  }

  # SonIndex  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SonIndex { NTree } {
    if { ! [ Empty $NTree ] } {
      set Father [ GetFather $NTree ]
      if { ! [ Empty $Father ] } {
        set TemporaryNTree [ GetFirstSon $Father ]
        set Index 1
        while { ! [ Empty $TemporaryNTree ] } {
          if { $NTree == $TemporaryNTree } {
            return $Index
          }
          set TemporaryNTree [ GetNextBrother $TemporaryNTree ]
          incr Index
        }
      } else {
        return 1
      }
    } else {
      return 0
    }
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
