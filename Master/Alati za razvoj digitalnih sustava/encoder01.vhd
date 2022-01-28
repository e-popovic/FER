library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity encoder01 is
   port  (
         clk               : in std_logic;
         reset             : in std_logic;
         
         rot_a             : in std_logic;
         rot_b             : in std_logic;
         
         rotary_event      : out std_logic;
         rotary_direction  : out std_logic
         );
end encoder01;

architecture RTL of encoder01 is

   -- local signals
   signal a_reg, b_reg                                : std_logic := '0';
   signal rotary_q1, rotary_q2                        : std_logic := '0';
   signal rotary_q1_delay, rot_event                  : std_logic := '0';
   signal rotary_event_local, rotary_direction_local  : std_logic := '0';

begin
   -- registering input signals
   process(clk)
   begin
      if rising_edge(clk) then
         if reset = '1' then
            a_reg <= '0';
            b_reg <= '0';
         else
            a_reg <= not rot_a;
            b_reg <= not rot_b;
         end if;
      end if;
   end process;

   -- generating rotary_q1 and rotary_q2
   process(clk)
      variable rotary_tmp  : std_logic_vector(1 downto 0);
   begin
      if rising_edge(clk) then
         if reset = '1' then
            rotary_q1 <= '0';
            rotary_q2 <= '0';
         else
            rotary_tmp := a_reg & b_reg;
            case rotary_tmp is
               when "00" =>   rotary_q1 <= '0';
                              rotary_q2 <= rotary_q2;
               when "01" =>   rotary_q1 <= rotary_q1;
                              rotary_q2 <= '1';
               when "10" =>   rotary_q1 <= rotary_q1;
                              rotary_q2 <= '0';
               when "11" =>   rotary_q1 <= '1';
                              rotary_q2 <= rotary_q2;
               when others => rotary_q1 <= rotary_q1;
                              rotary_q2 <= rotary_q2;
            end case;
         end if;
      end if;
   end process;
   
   -- registering rotary_q1
   process(clk)
   begin
      if rising_edge(clk) then
         if reset = '1' then
            rotary_q1_delay <= '0';
         else
            rotary_q1_delay <= rotary_q1;
         end if;
      end if;
   end process;
   
   -- generating rotary_event pulse
   rot_event <= rotary_q1 and not rotary_q1_delay;
   process(clk)
   begin
      if rising_edge(clk) then
         if reset = '1' then
            rotary_event_local <= '0';
         else
            rotary_event_local <= rot_event;
         end if;
      end if;
   end process;

   -- rotary_direction
   process(clk)
   begin
      if rising_edge(clk) then
         if reset = '1' then
            rotary_direction_local <= '0';
         elsif rot_event = '1' then
            rotary_direction_local <= rotary_q2;
         else
            rotary_direction_local <= rotary_direction_local;
         end if;
      end if;
   end process;

   -- output mapping
   rotary_event <= rotary_event_local;
   rotary_direction  <= rotary_direction_local;
end RTL;

