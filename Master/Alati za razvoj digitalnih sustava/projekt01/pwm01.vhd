----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Ema Popovic
-- 
-- Create Date:    08:20:39 11/15/2021 
-- Design Name: 
-- Module Name:    pwm01 - RTL 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity pwm01 is
	port (
		clk : in std_logic; -- takt
		rst : in std_logic; -- sinkroni reset
		N : in std_logic_vector(7 downto 0); -- upravljacki signal
		pwm : out std_logic -- PWM signal
	);
end pwm01;

architecture RTL of pwm01 is

	signal A, B: std_logic_vector(7 downto 0);

begin
	
	--D registar
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (rst = '1') then
				A <= "00000000";
			else
				A <= N;
			end if;
		end if;
	end process;
	
	--sinkrono brojilo
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (rst = '1') then
				B <= "11111111";
			else
				B <= B + 1;
			end if;
		end if;
	end process;
	
	--komparator
	process (A,B) is
	begin
		if (A > B) then
			pwm <= '1';
		else
			pwm <= '0';
		end if;
	end process;
	
end RTL;

