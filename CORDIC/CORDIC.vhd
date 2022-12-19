library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity CORDIC is
	port(
		phi : in signed(15 downto 0);
		n : in natural range 0 to 16;
		start : in std_logic;
		rst : in std_logic;
		clk : in std_logic;
		rdy : out std_logic;
		r1 : out signed(15 downto 0);
		r2 : out signed(15 downto 0));
end entity;


architecture RTL of CORDIC is
	type t_state is (Repos,Calcul);
	signal state : t_state;
	type tab is array(0 to 17) of signed (15 downto 0);
	--Declaration des tableaux
	signal TabTheta : tab := (
		to_signed(12149, 16), to_signed(7172, 16), to_signed(3789, 16), 
		to_signed(1923, 16), to_signed(965, 16), to_signed(483, 16), to_signed(241, 16), 
		to_signed(120, 16), to_signed(60, 16), to_signed(30, 16), to_signed(15, 16), 
		to_signed(7, 16), to_signed(3, 16), to_signed(1, 16), to_signed(0, 16), to_signed(0, 16),
		to_signed(0, 16),to_signed(0, 16));
		
	signal TabAn : tab := (
		to_signed(22627, 16), to_signed(20238, 16), to_signed(19634, 16), 
		to_signed(19482, 16), to_signed(19444, 16), to_signed(19435, 16), to_signed(19432, 16), 
		to_signed(19432, 16), to_signed(19432, 16), to_signed(19432, 16), to_signed(19432, 16), 
		to_signed(19432, 16), to_signed(19432, 16), to_signed(19432, 16), to_signed(19432, 16),
		to_signed(19432, 16),to_signed(19432, 16),to_signed(19432, 16));
		
		
	--Declaration des signaux utilisé dans la machine
	signal i : natural range 0 to 16; -- Meme taille que n
	signal x : signed(15 downto 0);
	signal y : signed(15 downto 0);
	signal alpha : signed(15 downto 0);
	signal theta : signed(15 downto 0);
	
begin
	process(clk,rst)
	begin
		if(rst = '1')then
			state <= Repos;
			rdy <= '1';
		elsif rising_edge(clk)then
			case state is
			when Repos =>
				if(start = '1')then
					state <= Calcul;
					x <= TabAn(n-1);
					y <= to_signed(0,16);
					alpha <= to_signed(0,16);
					theta <= TabTheta(0);
					i <= 0;				
				end if;
			when Calcul =>
				if(i < n)then
					if(alpha < phi)then
						x <= x - shift_right(y, i);
						y <= y + shift_right(x ,i);
						alpha <= alpha + theta;
					else 
						x <= x + shift_right(y, i);
						y <= y - shift_right(x ,i);
						alpha <= alpha - theta;
					end if;
					theta <= TabTheta(i+1);
					i <= i+1;
				else
					r1 <= x;
					r2 <= y;
					rdy <= '1';
					state <= Repos; 
				end if;
			end case;
		end if;
	end process;
end architecture;