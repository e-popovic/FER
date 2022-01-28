----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:11:46 12/17/2021 
-- Design Name: 
-- Module Name:    led01 - RTL 
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity led01 is
	port (
		clk : in std_logic; -- signal takta
		reset : in std_logic; -- signal reseta
		ld7 : out std_logic; -- signal za upravljanje LED LD7
		
		rot_a : in std_logic;
		rot_b : in std_logic;
		ld6 : out std_logic;
		
		ld1, ld2, ld3, ld4: out std_logic;
		nacin_rada: in std_logic
	);
end led01;

architecture RTL of led01 is

	signal resets, rot_direction, rot_event, dopustenje : std_logic;
	signal pomak: std_logic_vector(7 downto 0) := "00000001";
	signal brojac, brojac2: std_logic_vector(25 downto 0) := (others=>'0');
	
	type Stanje is (Svijetli1, Svijetli2, Svijetli3, Svijetli4, SvijetliSve, SvijetliNista);
	signal omoguci_prijelaz, nr, Q1, Q2 : std_logic;
	signal trenutno, sljedece: Stanje;
	
	component encoder01
	port (
			clk               : in std_logic;
         reset             : in std_logic;        
         rot_a             : in std_logic;
         rot_b             : in std_logic;        
         rotary_event      : out std_logic;
         rotary_direction  : out std_logic
			);
	end component;

begin

	enc: component encoder01 port map (clk => clk,
												reset => reset,
												rot_a => rot_a,
												rot_b => rot_b,
												rotary_event => rot_event,
												rotary_direction => rot_direction);

	--reset
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (reset='1') then
				resets <= '1';
			else
				resets <= '0';
			end if;
		end if;
	end process;
	
	--paljenje ledice 7
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (resets='1') then
				brojac <= (others=>'0');
			else
				brojac <= brojac + 1;
			end if;
		end if;
	end process;
	
	ld7 <= brojac(25);
	
	--brojac za ledicu 6
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (resets='1') then
				brojac2 <= (others=>'0');
			else
				brojac2 <= brojac2 + pomak;
			end if;
		end if;
	end process;
	
	--smije li se pomak mijenjati
	process (rot_event, rot_direction, pomak) is 
	begin
		if (rot_event='1') then
			if (rot_direction='1') and (pomak(7)='0') then	--desno, provjera dal smije jos ubrzavati
				dopustenje <= '1';
			elsif (rot_direction='0') and (pomak(0)='0') then	--lijevo, provjera dal smije jos usporavati
				dopustenje <='1';
			else
				dopustenje <= '0';
			end if;
		else
			dopustenje <= '0';
		end if;
	end process;
	
	--odredivanje pomaka za ledicu 6
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (resets='1') then
				pomak <= "00000001";
			elsif (dopustenje='1') then
				if (rot_direction='1') then		--desno
					pomak <= pomak(6 downto 0) & '0';
				elsif (rot_direction='0') then		--lijevo
					pomak <= '0' & pomak(7 downto 1);
				end if;
			end if;
		end if;
	end process;
	
	ld6 <= brojac2(25);
	
	--nacin rada
	process (clk) is
	begin
		if (rising_edge(clk)) then
			if (resets='1') then
				nr <= '0';
			else
				nr <= nacin_rada;
			end if;
		end if;
	end process;
	
	--generiranje pulseva
	process (clk) is
	begin
		if rising_edge(clk) then
			if reset='1' then
				Q1<='0';
				Q2<='0';
			else
				Q2<=Q1;
				Q1<=brojac(25);
			end if;
		end if;
	end process;
	
	omoguci_prijelaz <= (not Q2) and Q1;
	
	
	--AUTOMAT
	
	-- sinkroni dio
	process (clk)
	begin
		if rising_edge(clk) then
			if reset='1' then -- sinkroni reset automata
				trenutno <= Svijetli1;
			else
				trenutno <= sljedece;
			end if;
		end if;
	end process;

	-- kombinacijski dio
	process (trenutno, nr, omoguci_prijelaz)
	begin
		ld1 <= '0';
		ld2 <= '0';
		ld3 <= '0';
		ld4 <= '0';
		
		case trenutno is
			--Svijetli1, Svijetli2, Svijetli3, Svijetli4, SvijetliSve, SvijetliNista
			when Svijetli1 =>
				ld1 <= '1';
				if omoguci_prijelaz = '1' then
					if nr = '1' then
						sljedece <= SvijetliSve;
					else
						sljedece <= Svijetli2;
					end if;
				else 
					sljedece <= Svijetli1;
				end if;
			
			when Svijetli2 =>
				ld2 <= '1';
				if omoguci_prijelaz = '1' then
					sljedece <= Svijetli3;
				else 
					sljedece <= Svijetli2;
				end if;
			
			when Svijetli3 =>
				ld3 <= '1';
				if omoguci_prijelaz = '1' then
					sljedece <= Svijetli4;
				else 
					sljedece <= Svijetli3;
				end if;
			
			
			when Svijetli4 =>
				ld4 <= '1';
				if omoguci_prijelaz = '1' then
					sljedece <= Svijetli1;
				else 
					sljedece <= Svijetli4;
				end if;
			
				
			when SvijetliSve =>
				ld1 <= '1';
				ld2 <= '1';
				ld3 <= '1';
				ld4 <= '1';
				if omoguci_prijelaz = '1' then
					sljedece <= SvijetliNista;
				else 
					sljedece <= SvijetliSve;
				end if;
			
			
			when SvijetliNista =>
				ld1 <= '0';
				ld2 <= '0';
				ld3 <= '0';
				ld4 <= '0';
				if omoguci_prijelaz = '1' then
					if nr = '1' then
						sljedece <= Svijetli3;
					else
						sljedece <= SvijetliSve;
					end if;
				else 
					sljedece <= SvijetliNista;
				end if;

			when others =>
				null;
		end case;
	end process;

end RTL;

