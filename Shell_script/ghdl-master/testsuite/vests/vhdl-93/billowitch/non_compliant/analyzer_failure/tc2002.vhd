
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
-- $Id: tc2002.vhd,v 1.2 2001-10-26 16:30:15 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------


--                 ****************************               --
-- Ported to VHDL 93 by port93.pl - Tue Nov  5 16:40:54 1996  --
--                 ****************************               --



--                 ****************************                   --
-- Reversed to VHDL 87 by reverse87.pl - Tue Nov  5 11:27:56 1996  --
--                 ****************************                    --



--                 ****************************               --
-- Ported to VHDL 93 by port93.pl - Mon Nov  4 17:35:51 1996  --
--                 ****************************               --


use std.textio.all;
ENTITY c07s02b02x00p07n01i02002ent IS
END c07s02b02x00p07n01i02002ent;

ARCHITECTURE c07s02b02x00p07n01i02002arch OF c07s02b02x00p07n01i02002ent IS

BEGIN
  TESTING: PROCESS
    file f1 : text open write_mode is "aout";
    file f2 : text open write_mode is "aout";
  BEGIN
    if f1 = f2 then
      null;
    end if;
    assert FALSE
      report "***FAILED TEST: c07s02b02x00p07n01i02002 - Equality operators are not defined for file types."
      severity ERROR;
    wait;
  END PROCESS TESTING;

END c07s02b02x00p07n01i02002arch;
