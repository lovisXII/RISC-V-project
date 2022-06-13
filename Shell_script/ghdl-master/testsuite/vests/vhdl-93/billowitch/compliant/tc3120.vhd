
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
-- $Id: tc3120.vhd,v 1.2 2001-10-26 16:29:51 paw Exp $
-- $Revision: 1.2 $
--
-- ---------------------------------------------------------------------

ENTITY c05s02b01x01p05n01i03120ent_a IS
  generic ( g1 : boolean );
  port    ( p1 : in  Bit;
            p2 : out Bit );
END c05s02b01x01p05n01i03120ent_a;

ARCHITECTURE c05s02b01x01p05n01i03120arch_a OF c05s02b01x01p05n01i03120ent_a IS

BEGIN
  p2 <= p1 after 10 ns;
END c05s02b01x01p05n01i03120arch_a;

configuration c05s02b01x01p05n01i03120cfg_a of c05s02b01x01p05n01i03120ent_a is
  for c05s02b01x01p05n01i03120arch_a
  end for;
end c05s02b01x01p05n01i03120cfg_a;


--


ENTITY c05s02b01x01p05n01i03120ent IS
END c05s02b01x01p05n01i03120ent;

ARCHITECTURE c05s02b01x01p05n01i03120arch OF c05s02b01x01p05n01i03120ent IS
  component virtual
    generic ( g1 : boolean );
    port    ( p1 : in  Bit;
              p2 : out Bit );
  end component;
  signal s1,s2,s3,s4 : Bit;
  for all : virtual use configuration work.c05s02b01x01p05n01i03120cfg_a;
BEGIN
  u1 : virtual
    generic map ( true )
    port map (s1, s2);
  u2 : virtual
    generic map ( true )
    port map (s2, s3);
  u3 : virtual
    generic map ( true )
    port map (s3, s4);

  TESTING: PROCESS
  BEGIN
    wait for 30 ns;
    assert NOT(    s2 = s1   and
                   s3 = s2   and
                   s4 = s3   )
      report "***PASSED TEST: c05s02b01x01p05n01i03120"
      severity NOTE;
    assert (    s2 = s1   and
                s3 = s2   and
                s4 = s3   )
      report "***FAILED TEST: c05s02b01x01p05n01i03120 - Use a configuration that is fully bound test failed."
      severity ERROR;
    wait;
  END PROCESS TESTING;

END c05s02b01x01p05n01i03120arch;


configuration c05s02b01x01p05n01i03120cfg of c05s02b01x01p05n01i03120ent is
  for c05s02b01x01p05n01i03120arch
  end for;
end c05s02b01x01p05n01i03120cfg;
