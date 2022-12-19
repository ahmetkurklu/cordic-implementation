library ieee;
use ieee.std_logic_1164.all;	   
use ieee.numeric_std.all;

entity tb is
end tb;

architecture seq of tb is

	signal phi : signed(15 downto 0);
	signal n : natural range 0 to 16;
	signal start : std_logic;
	signal rst : std_logic;
	signal clk : std_logic;
	signal rdy : std_logic;
	signal r1 : signed(15 downto 0);
	signal r2 : signed(15 downto 0);


begin

  UUT: entity work.CORDIC port map(phi,n,start,rst,clk,rdy,r1,r2);
  
  CLOCK: process
  begin
    clk<='1';
    wait for 5 ns;
    clk<='0';
    wait for 5 ns;
  end process;

  RESET:process
  begin
    rst <= '1'; 
    wait for 3 ns;
    rst <= '0';
    wait;
  end process;

  MISEPLACE:process
  begin
    phi <= to_signed(40*270,16);
    n <= 15;
    start <='0';
    wait for 30 ns;
    start <= '1';
    wait for 30 ns;
    start <= '0';
    wait;
  end process;
  
  


end architecture;
