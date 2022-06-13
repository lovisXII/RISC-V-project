
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
-- $Id: tc984.vhd,v 1.2 2001-10-26 16:30:29 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------

ENTITY c06s03b00x00p06n01i00984ent IS
END c06s03b00x00p06n01i00984ent;

ARCHITECTURE c06s03b00x00p06n01i00984arch OF c06s03b00x00p06n01i00984ent IS

BEGIN
  TESTING: PROCESS
    type some_record is
      record
        x : integer;
        y : boolean;
      end record;

    type some_ptr is access some_record;

    variable some_var : some_ptr;
    variable some_rec : some_record;
  BEGIN
    some_rec := some_var; --  should be some_rec := some_var.all
    assert FALSE 
      report "***FAILED TEST: c06s03b00x00p06n01i00984 - Suffix of a selected name must be the reserved word all." 
      severity ERROR;
    wait;
  END PROCESS TESTING;

END c06s03b00x00p06n01i00984arch;
