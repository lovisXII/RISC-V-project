library ieee;
use ieee.std_logic_1164.all;

entity cmp_325 is
	port (
		eq : out std_logic;
		in0 : in  std_logic;
		in1 : in  std_logic
	);
end cmp_325;

architecture augh of cmp_325 is

	signal tmp : std_logic;

begin

	-- Compute the result
	tmp <=
		'0' when in0 /= in1 else
		'1';

	-- Set the outputs
	eq <= tmp;

end architecture;
