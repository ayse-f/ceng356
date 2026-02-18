library ieee;
use ieee.std_logic_1164.all;

entity ALU is
  generic (n : integer);
  --generic (n : integer := 6);  -- for synthesis
  port (A    : in std_logic_vector((n-1) downto 0);
        B    : in std_logic_vector((n-1) downto 0);
        S    : in std_logic_vector(2 downto 0);

        RSLT : out std_logic_vector((n-1) downto 0);
        OVF  : out std_logic;
        CF   : out std_logic;
        NF   : out std_logic;
        ZF   : out std_logic);
end ALU;

architecture RTL of ALU is
  
  component CLA
    generic (n : integer );
    port (A    : in std_logic_vector((n-1) downto 0);
          B    : in std_logic_vector((n-1) downto 0);
          CIN  : in std_logic;
  
          COUT : out std_logic;
          SUM  : out std_logic_vector((n-1) downto 0) );
  end component;
    
  component Mux
    generic (n : integer);
    port (D0   : in std_logic_vector((n-1) downto 0);
          D1   : in std_logic_vector((n-1) downto 0);
          D2   : in std_logic_vector((n-1) downto 0);
          D3   : in std_logic_vector((n-1) downto 0);
          D4   : in std_logic_vector((n-1) downto 0);
          D5   : in std_logic_vector((n-1) downto 0);
          D6   : in std_logic_vector((n-1) downto 0);
          D7   : in std_logic_vector((n-1) downto 0);
          SEL  : in std_logic_vector(2 downto 0);
  
          Y    : out std_logic_vector((n-1) downto 0));
  end component;
  
-- Declare signals you deem necessary to interconnect all the components declared above.
  signal C_l    : std_logic;
  signal SUb    : std_logic;
  signal B_l    : std_logic_vector((n-1) downto 0);
  signal Sum    : std_logic_vector((n-1) downto 0);
  signal R_And  : std_logic_vector((n-1) downto 0);
  signal R_Or   : std_logic_vector((n-1) downto 0);
  signal R_Xor  : std_logic_vector((n-1) downto 0);
  signal NotA   : std_logic_vector((n-1) downto 0);
  signal R_Shl  : std_logic_vector((n-1) downto 0);
  signal R_Shr  : std_logic_vector((n-1) downto 0);
  signal Zero   : std_logic_vector((n-1) downto 0);

begin

  Zero <= (others => '0');

-- Complete the following statements for logic operations:
  R_And <= A and B;                     -- AND operation
  R_Or  <= A or B;                     -- OR  operation
  R_Xor <= A xor B;                     -- XOR operation
  NotA  <= not A;                     -- NOT operation

-- The following statements are for shift operations:
  R_Shr <= '0' & A((n-1) downto 1);     -- Logical Right shift
  R_Shl <= A((n-2) downto 0) & '0';     -- Logical Left  shift

-- Complete the port map below by using signals you declared above
-- to perform both addition and subtraction by using an adder.
  Sub  <= '1' when S = "001" else '0';  -- If S=001, perform subtraction
  B_l  <= NOT B when Sub = '1' else B;  -- In case of subtraction, prepare 1's compliment,
					-- which is converted to 2's compliment by setting CIN to 1.
                                        -- In case of addition, this signal (B_l) = data input B
                                        
  AddSub : CLA
    generic map (n => n)
    port map (A => A,                 -- Connect data input A
	      B => B_l,                 -- Connect data input B for addition or 1's compliment of B for subtraction
	      CIN => Sub,                 -- Should be 0 for addition; 1 for subtraction
          COUT => C_l,
	      SUM  => Sum);             -- This is the output from the adder/subtractor

    
-- Complete the following statements to generate condition flags.
  OVF <= C_l xor Sum(n-1) xor A(n-1) xor B_l(n-1);
  CF <= C_l;
  NF <= Sum(n-1);
  ZF <= '1' when Sum = Zero else '0';

-- Complete the port map below by using signals you declared above
-- to select the proper result based on the control input, S. 

  DataSelect : MUX
    generic map (n => n)
    port map (D0 => Sum,                 -- Refer to the function table to understand
	          D1 => Sum,                 -- which data input is to be selected for which control input.
              D2 => R_And,                 
              D3 => R_Or,
              D4 => R_Xor,
              D5 => NotA,
              D6 => R_Shr,
              D7 => R_Shl,
	          SEL => S,                -- This is the control input for data selection
	          Y   => RSLT);           -- This is the final result from the ALU

end RTL;
