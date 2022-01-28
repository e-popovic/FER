--------------------------------------------------------------------------------
-- Company: 
-- Engineer: Ema Popovic
--
-- Create Date:   08:46:37 11/15/2021
-- Design Name:   
-- Module Name:   D:/Stvari/Faks/Alati za razvoj digitalnih sustava/azrds/Popovic/projekt01/pwm01_tb.vhd
-- Project Name:  projekt01
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: pwm01
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY pwm01_tb IS
END pwm01_tb;
 
ARCHITECTURE behavior OF pwm01_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT pwm01
    PORT(
         clk : IN  std_logic;
         rst : IN  std_logic;
         N : IN  std_logic_vector(7 downto 0);
         pwm : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal rst : std_logic := '0';
   signal N : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal pwm : std_logic;
	
	-- Clock period counter
	signal counter : std_logic_vector(7 downto 0);
 
BEGIN
 
	-- N=2 od 0 do 800us, N=7 od 800us do 2000us i N=10 od 2000us do
	-- kraja simulacije
	N <= "11111111", "00000111" after 800 us, "00001010" after 2000 us;
	rst <= '1', '0' after 10500 ns;
		
	-- Instantiate the Unit Under Test (UUT)
   uut: pwm01 PORT MAP (
          clk => clk,
          rst => rst,
          N => N,
          pwm => pwm
        );

   -- Clock process definitions
	-- 1 MHz
   clk_process :process
   begin
		clk <= '0';
		wait for 500 ns;
		clk <= '1';
		wait for 500 ns;
   end process;
	
	-- Counter process
	counter_process: process
	begin
		wait until rising_edge(clk);
		if (pwm = '1') then
			counter <= counter + 1;
		else
			counter <= "00000000";
		end if;		
	end process;
	
	-- Assert process
	assert_process: process
	begin
		wait until falling_edge(pwm);	
		if (rst = '0') then
			assert (counter = N)
				report "PWM ne radi!"
				severity ERROR;
		end if;
	end process;

END;
