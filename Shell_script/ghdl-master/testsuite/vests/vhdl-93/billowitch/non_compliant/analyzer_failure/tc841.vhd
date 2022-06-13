
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
-- $Id: tc841.vhd,v 1.2 2001-10-26 16:30:28 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------

entity c01s03b01x00p04n01i00841ent_a is
end c01s03b01x00p04n01i00841ent_a;

architecture c01s03b01x00p04n01i00841arch_a of c01s03b01x00p04n01i00841ent_a is
begin
end c01s03b01x00p04n01i00841arch_a;

ENTITY c01s03b01x00p04n01i00841ent IS
  port (N : integer);
END c01s03b01x00p04n01i00841ent;

ARCHITECTURE c01s03b01x00p04n01i00841arch OF c01s03b01x00p04n01i00841ent IS

BEGIN
  
  AA_BLK : block
    component FOUR
    end component;
  begin
    LH : FOUR;
    LR : FOUR;
    aaa_blk: block
    begin
    end block;
    L1: for I in 1 to 3 generate
    end generate;
  end block;

  TESTING: PROCESS
  BEGIN
    assert FALSE 
      report "***FAILED TEST: c01s03b01x00p04n01i00841 - Index specification is not locally static."
      severity NOTE;
    wait;
  END PROCESS TESTING;

END c01s03b01x00p04n01i00841arch;

configuration c01s03b01x00p04n01i00841cfg of c01s03b01x00p04n01i00841ent is
  for c01s03b01x00p04n01i00841arch
    for AA_BLK
      for LH, LR : FOUR
        use entity work.c01s03b01x00p04n01i00841ent_a(c01s03b01x00p04n01i00841_arch_a);
      end for;
      for aaa_blk
      end for;
      for L1 (1 to N)                 --- No_failure_here
      end for;
      for L1 (3)
      end for;
    end for;
  end for;
end c01s03b01x00p04n01i00841cfg;
