
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
-- $Id: tc537.vhd,v 1.2 2001-10-26 16:29:56 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------

ENTITY c03s03b00x00p05n02i00537ent IS
END c03s03b00x00p05n02i00537ent;

ARCHITECTURE c03s03b00x00p05n02i00537arch OF c03s03b00x00p05n02i00537ent IS
  type ARR is access BIT_VECTOR ;
BEGIN
  TESTING: PROCESS
    variable V1 : ARR := null ;
    variable V2 : ARR(0 to 3) := new BIT_VECTOR'("1111") ; -- no_failure_here
  BEGIN
    V1 := V2;
    assert NOT(V1(0 to 3)="1111")
      report "***PASSED TEST: c03s03b00x00p05n02i00537"
      severity NOTE;
    assert (V1(0 to 3)="1111")
      report "***FAILED TEST: c03s03b00x00p05n02i00537 - An access value belongs to a corresponding subtype of an access type if the value of the designated object satisfies the constraint."
      severity ERROR;
    wait;
  END PROCESS TESTING;

END c03s03b00x00p05n02i00537arch;
