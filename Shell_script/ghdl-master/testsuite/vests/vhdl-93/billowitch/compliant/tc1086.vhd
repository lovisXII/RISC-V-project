
-- Copyright (C) 2001 Bill Billowitch.

-- Some of the work to develop this test suite was done with Air Force
-- support.  The Air Force and Bill Billowitch assume no
-- responsibilities for this software.

-- This file is part of VESTs (Vhdl tESTs).

-- VESTs is free software; you can redistribute it and/or modify it
-- under the terms of the GNU General Public License as published by the
-- Free Software Foundation; either version 2 of the License, or (at
-- your option) any later version. 

-- VESTs is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
-- for more details. 

-- You should have received a copy of the GNU General Public License
-- along with VESTs; if not, write to the Free Software Foundation,
-- Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 

-- ---------------------------------------------------------------------
--
-- $Id: tc1086.vhd,v 1.2 2001-10-26 16:29:38 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------

ENTITY c06s05b00x00p02n01i01086ent IS
END c06s05b00x00p02n01i01086ent;

ARCHITECTURE c06s05b00x00p02n01i01086arch OF c06s05b00x00p02n01i01086ent IS

BEGIN
  TESTING: PROCESS
    variable str : string (1 to 25) := "This is array slice check";
    variable k   : integer;
  BEGIN
    if str(1 to 3) = "Thi" then -- Success_here
      k := 5;
    end if;
    assert NOT(k=5) 
      report "***PASSED TEST: c06s05b00x00p02n01i01086" 
      severity NOTE;
    assert (k=5) 
      report "***FAILED TEST: c06s05b00x00p02n01i01086 - Slice name consists of a single discrete range enclosed within parentheses."
      severity ERROR;
    wait;
  END PROCESS TESTING;

END c06s05b00x00p02n01i01086arch;
