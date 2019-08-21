library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;

use work.algo_pkg.all;

entity algo_top_wrapper is
  port (
    ap_clk   : in  std_logic;
    ap_rst   : in  std_logic;
    ap_start : in  std_logic;
    ap_done  : out std_logic;
    ap_idle  : out std_logic;
    ap_ready : out std_logic;

    -- Input Links 
    link_in_master : in  LinkMasterArrType(47 downto 0);
    link_in_slave  : out LinkSlaveArrType(47 downto 0);

    -- Output Links 
    link_out_master : out LinkMasterArrType(47 downto 0);
    link_out_slave  : in  LinkSlaveArrType(47 downto 0)
    );

end algo_top_wrapper;

architecture rtl of algo_top_wrapper is

  type t_cyc_3_arr is array(integer range <>) of integer range 0 to 2;
  type t_slv_192_arr is array(integer range <>) of std_logic_vector(191 downto 0);

  component algo_unpacked
    port (
      ap_clk   : in  std_logic;
      ap_rst   : in  std_logic;
      ap_start : in  std_logic;
      ap_done  : out std_logic;
      ap_idle  : out std_logic;
      ap_ready : out std_logic;

      link_in_0_V  : in std_logic_vector (191 downto 0);
      link_in_1_V  : in std_logic_vector (191 downto 0);
      link_in_2_V  : in std_logic_vector (191 downto 0);
      link_in_3_V  : in std_logic_vector (191 downto 0);
      link_in_4_V  : in std_logic_vector (191 downto 0);
      link_in_5_V  : in std_logic_vector (191 downto 0);
      link_in_6_V  : in std_logic_vector (191 downto 0);
      link_in_7_V  : in std_logic_vector (191 downto 0);
      link_in_8_V  : in std_logic_vector (191 downto 0);
      link_in_9_V  : in std_logic_vector (191 downto 0);
      link_in_10_V : in std_logic_vector (191 downto 0);
      link_in_11_V : in std_logic_vector (191 downto 0);
      link_in_12_V : in std_logic_vector (191 downto 0);
      link_in_13_V : in std_logic_vector (191 downto 0);
      link_in_14_V : in std_logic_vector (191 downto 0);
      link_in_15_V : in std_logic_vector (191 downto 0);
      link_in_16_V : in std_logic_vector (191 downto 0);
      link_in_17_V : in std_logic_vector (191 downto 0);
      link_in_18_V : in std_logic_vector (191 downto 0);
      link_in_19_V : in std_logic_vector (191 downto 0);
      link_in_20_V : in std_logic_vector (191 downto 0);
      link_in_21_V : in std_logic_vector (191 downto 0);
      link_in_22_V : in std_logic_vector (191 downto 0);
      link_in_23_V : in std_logic_vector (191 downto 0);
      link_in_24_V : in std_logic_vector (191 downto 0);
      link_in_25_V : in std_logic_vector (191 downto 0);
      link_in_26_V : in std_logic_vector (191 downto 0);
      link_in_27_V : in std_logic_vector (191 downto 0);
      link_in_28_V : in std_logic_vector (191 downto 0);
      link_in_29_V : in std_logic_vector (191 downto 0);
      link_in_30_V : in std_logic_vector (191 downto 0);
      link_in_31_V : in std_logic_vector (191 downto 0);
      link_in_32_V : in std_logic_vector (191 downto 0);
      link_in_33_V : in std_logic_vector (191 downto 0);
      link_in_34_V : in std_logic_vector (191 downto 0);
      link_in_35_V : in std_logic_vector (191 downto 0);
      link_in_36_V : in std_logic_vector (191 downto 0);
      link_in_37_V : in std_logic_vector (191 downto 0);
      link_in_38_V : in std_logic_vector (191 downto 0);
      link_in_39_V : in std_logic_vector (191 downto 0);
      link_in_40_V : in std_logic_vector (191 downto 0);
      link_in_41_V : in std_logic_vector (191 downto 0);
      link_in_42_V : in std_logic_vector (191 downto 0);
      link_in_43_V : in std_logic_vector (191 downto 0);
      link_in_44_V : in std_logic_vector (191 downto 0);
      link_in_45_V : in std_logic_vector (191 downto 0);
      link_in_46_V : in std_logic_vector (191 downto 0);
      link_in_47_V : in std_logic_vector (191 downto 0);

      link_out_0_V  : out std_logic_vector (191 downto 0);
      link_out_1_V  : out std_logic_vector (191 downto 0);
      link_out_2_V  : out std_logic_vector (191 downto 0);
      link_out_3_V  : out std_logic_vector (191 downto 0);
      link_out_4_V  : out std_logic_vector (191 downto 0);
      link_out_5_V  : out std_logic_vector (191 downto 0);
      link_out_6_V  : out std_logic_vector (191 downto 0);
      link_out_7_V  : out std_logic_vector (191 downto 0);
      link_out_8_V  : out std_logic_vector (191 downto 0);
      link_out_9_V  : out std_logic_vector (191 downto 0);
      link_out_10_V : out std_logic_vector (191 downto 0);
      link_out_11_V : out std_logic_vector (191 downto 0);
      link_out_12_V : out std_logic_vector (191 downto 0);
      link_out_13_V : out std_logic_vector (191 downto 0);
      link_out_14_V : out std_logic_vector (191 downto 0);
      link_out_15_V : out std_logic_vector (191 downto 0);
      link_out_16_V : out std_logic_vector (191 downto 0);
      link_out_17_V : out std_logic_vector (191 downto 0);
      link_out_18_V : out std_logic_vector (191 downto 0);
      link_out_19_V : out std_logic_vector (191 downto 0);
      link_out_20_V : out std_logic_vector (191 downto 0);
      link_out_21_V : out std_logic_vector (191 downto 0);
      link_out_22_V : out std_logic_vector (191 downto 0);
      link_out_23_V : out std_logic_vector (191 downto 0);
      link_out_24_V : out std_logic_vector (191 downto 0);
      link_out_25_V : out std_logic_vector (191 downto 0);
      link_out_26_V : out std_logic_vector (191 downto 0);
      link_out_27_V : out std_logic_vector (191 downto 0);
      link_out_28_V : out std_logic_vector (191 downto 0);
      link_out_29_V : out std_logic_vector (191 downto 0);
      link_out_30_V : out std_logic_vector (191 downto 0);
      link_out_31_V : out std_logic_vector (191 downto 0);
      link_out_32_V : out std_logic_vector (191 downto 0);
      link_out_33_V : out std_logic_vector (191 downto 0);
      link_out_34_V : out std_logic_vector (191 downto 0);
      link_out_35_V : out std_logic_vector (191 downto 0);
      link_out_36_V : out std_logic_vector (191 downto 0);
      link_out_37_V : out std_logic_vector (191 downto 0);
      link_out_38_V : out std_logic_vector (191 downto 0);
      link_out_39_V : out std_logic_vector (191 downto 0);
      link_out_40_V : out std_logic_vector (191 downto 0);
      link_out_41_V : out std_logic_vector (191 downto 0);
      link_out_42_V : out std_logic_vector (191 downto 0);
      link_out_43_V : out std_logic_vector (191 downto 0);
      link_out_44_V : out std_logic_vector (191 downto 0);
      link_out_45_V : out std_logic_vector (191 downto 0);
      link_out_46_V : out std_logic_vector (191 downto 0);
      link_out_47_V : out std_logic_vector (191 downto 0);

      link_out_0_V_ap_vld  : out std_logic;
      link_out_1_V_ap_vld  : out std_logic;
      link_out_2_V_ap_vld  : out std_logic;
      link_out_3_V_ap_vld  : out std_logic;
      link_out_4_V_ap_vld  : out std_logic;
      link_out_5_V_ap_vld  : out std_logic;
      link_out_6_V_ap_vld  : out std_logic;
      link_out_7_V_ap_vld  : out std_logic;
      link_out_8_V_ap_vld  : out std_logic;
      link_out_9_V_ap_vld  : out std_logic;
      link_out_10_V_ap_vld : out std_logic;
      link_out_11_V_ap_vld : out std_logic;
      link_out_12_V_ap_vld : out std_logic;
      link_out_13_V_ap_vld : out std_logic;
      link_out_14_V_ap_vld : out std_logic;
      link_out_15_V_ap_vld : out std_logic;
      link_out_16_V_ap_vld : out std_logic;
      link_out_17_V_ap_vld : out std_logic;
      link_out_18_V_ap_vld : out std_logic;
      link_out_19_V_ap_vld : out std_logic;
      link_out_20_V_ap_vld : out std_logic;
      link_out_21_V_ap_vld : out std_logic;
      link_out_22_V_ap_vld : out std_logic;
      link_out_23_V_ap_vld : out std_logic;
      link_out_24_V_ap_vld : out std_logic;
      link_out_25_V_ap_vld : out std_logic;
      link_out_26_V_ap_vld : out std_logic;
      link_out_27_V_ap_vld : out std_logic;
      link_out_28_V_ap_vld : out std_logic;
      link_out_29_V_ap_vld : out std_logic;
      link_out_30_V_ap_vld : out std_logic;
      link_out_31_V_ap_vld : out std_logic;
      link_out_32_V_ap_vld : out std_logic;
      link_out_33_V_ap_vld : out std_logic;
      link_out_34_V_ap_vld : out std_logic;
      link_out_35_V_ap_vld : out std_logic;
      link_out_36_V_ap_vld : out std_logic;
      link_out_37_V_ap_vld : out std_logic;
      link_out_38_V_ap_vld : out std_logic;
      link_out_39_V_ap_vld : out std_logic;
      link_out_40_V_ap_vld : out std_logic;
      link_out_41_V_ap_vld : out std_logic;
      link_out_42_V_ap_vld : out std_logic;
      link_out_43_V_ap_vld : out std_logic;
      link_out_44_V_ap_vld : out std_logic;
      link_out_45_V_ap_vld : out std_logic;
      link_out_46_V_ap_vld : out std_logic;
      link_out_47_V_ap_vld : out std_logic

      );
  end component algo_unpacked;

  signal ap_rst_d1 : std_logic;
  signal ap_rst_d2 : std_logic;
  signal ap_rst_d3 : std_logic;
  signal ap_rst_d4 : std_logic;
  signal ap_rst_d5 : std_logic;

  signal ap_clk_unp   : std_logic;
  signal ap_rst_unp   : std_logic;
  signal ap_start_unp : std_logic;
  signal ap_done_unp  : std_logic;
  signal ap_idle_unp  : std_logic;
  signal ap_ready_unp : std_logic;

  signal link_in      : t_slv_192_arr(47 downto 0);
  signal link_out     : t_slv_192_arr(47 downto 0);
  
  signal link_in_reg  : t_slv_192_arr(47 downto 0);
  signal link_out_reg : t_slv_192_arr(47 downto 0);

  signal link_out_ap_vld : std_logic_vector(47 downto 0);
  signal link_out_ap_vld_latched : std_logic_vector(47 downto 0);

  signal in_cyc  : t_cyc_3_arr(47 downto 0);
  signal out_cyc : t_cyc_3_arr(47 downto 0);

begin

  ap_done <= ap_done_unp;
  
  process(ap_clk) is
  begin
       if rising_edge(ap_clk) then
         ap_rst_d1 <= ap_rst;
         ap_rst_d2 <= ap_rst_d1;
         ap_rst_d3 <= ap_rst_d2;
         ap_rst_d4 <= ap_rst_d3;
         ap_rst_d5 <= ap_rst_d4;
     end if;
  end process;
  
  gen_cyc : for idx in 0 to 47 generate
    process(ap_clk) is
    begin
      if rising_edge(ap_clk) then

        if (link_out_ap_vld(idx) = '1') then
          link_out_reg(idx) <= link_out(idx);
        end if;

        if (ap_rst_d2 = '1') then
          in_cyc(idx) <= 0;
        else
          in_cyc(idx) <= in_cyc(idx) + 1;
          if (in_cyc(idx) = 2) then
            in_cyc(idx) <= 0;
          end if;
        end if;

        if (ap_rst = '1') then
          link_out_ap_vld_latched(idx) <= '0';
        elsif (link_out_ap_vld(idx) = '1') then
          link_out_ap_vld_latched(idx) <= '1';
        end if;

        if (link_out_ap_vld_latched(idx) = '0') then
          out_cyc(idx)                <= 0;
          link_out_master(idx).tvalid <= '0';
        else
          link_out_master(idx).tvalid <= '1';
          out_cyc(idx)                <= out_cyc(idx) + 1;
          if (out_cyc(idx) = 2) then
            out_cyc(idx) <= 0;
          end if;
        end if;

        if (in_cyc(idx) = 0) then 
            link_in(idx)(63 downto 0)    <= link_in_master(idx).tdata; 
        end if;
        
        if (in_cyc(idx) = 1) then 
            link_in(idx)(127 downto 64)  <= link_in_master(idx).tdata; 
        end if;
        
        if (in_cyc(idx) = 2) then 
            link_in_reg(idx)(63 downto 0) <= link_in(idx)(63 downto 0);
            link_in_reg(idx)(127 downto 64) <= link_in(idx)(127 downto 64);
            link_in_reg(idx)(191 downto 128) <= link_in_master(idx).tdata; 
        end if;

        if (out_cyc(idx) = 0) then link_out_master(idx).tdata <= link_out_reg(idx)(63 downto 0); end if;
        if (out_cyc(idx) = 1) then link_out_master(idx).tdata <= link_out_reg(idx)(127 downto 64); end if;
        if (out_cyc(idx) = 2) then link_out_master(idx).tdata <= link_out_reg(idx)(191 downto 128); end if;

      end if;
    end process;
  end generate;

  ap_clk_unp   <= ap_clk;
  ap_rst_unp   <= ap_rst;
  ap_start_unp <= not ap_rst_d5;
  ap_idle      <= ap_idle_unp;
  ap_ready     <= ap_ready_unp;

  i_algo_unpacked : algo_unpacked
    port map (
      ap_clk   => ap_clk_unp,
      ap_rst   => ap_rst_unp,
      ap_start => ap_start_unp,
      ap_done  => ap_done_unp,
      ap_idle  => ap_idle_unp,
      ap_ready => ap_ready_unp,

      link_in_0_V  => link_in_reg(0),
      link_in_1_V  => link_in_reg(1),
      link_in_2_V  => link_in_reg(2),
      link_in_3_V  => link_in_reg(3),
      link_in_4_V  => link_in_reg(4),
      link_in_5_V  => link_in_reg(5),
      link_in_6_V  => link_in_reg(6),
      link_in_7_V  => link_in_reg(7),
      link_in_8_V  => link_in_reg(8),
      link_in_9_V  => link_in_reg(9),
      link_in_10_V => link_in_reg(10),
      link_in_11_V => link_in_reg(11),
      link_in_12_V => link_in_reg(12),
      link_in_13_V => link_in_reg(13),
      link_in_14_V => link_in_reg(14),
      link_in_15_V => link_in_reg(15),
      link_in_16_V => link_in_reg(16),
      link_in_17_V => link_in_reg(17),
      link_in_18_V => link_in_reg(18),
      link_in_19_V => link_in_reg(19),
      link_in_20_V => link_in_reg(20),
      link_in_21_V => link_in_reg(21),
      link_in_22_V => link_in_reg(22),
      link_in_23_V => link_in_reg(23),
      link_in_24_V => link_in_reg(24),
      link_in_25_V => link_in_reg(25),
      link_in_26_V => link_in_reg(26),
      link_in_27_V => link_in_reg(27),
      link_in_28_V => link_in_reg(28),
      link_in_29_V => link_in_reg(29),
      link_in_30_V => link_in_reg(30),
      link_in_31_V => link_in_reg(31),
      link_in_32_V => link_in_reg(32),
      link_in_33_V => link_in_reg(33),
      link_in_34_V => link_in_reg(34),
      link_in_35_V => link_in_reg(35),
      link_in_36_V => link_in_reg(36),
      link_in_37_V => link_in_reg(37),
      link_in_38_V => link_in_reg(38),
      link_in_39_V => link_in_reg(39),
      link_in_40_V => link_in_reg(40),
      link_in_41_V => link_in_reg(41),
      link_in_42_V => link_in_reg(42),
      link_in_43_V => link_in_reg(43),
      link_in_44_V => link_in_reg(44),
      link_in_45_V => link_in_reg(45),
      link_in_46_V => link_in_reg(46),
      link_in_47_V => link_in_reg(47),

      link_out_0_V  => link_out(0),
      link_out_1_V  => link_out(1),
      link_out_2_V  => link_out(2),
      link_out_3_V  => link_out(3),
      link_out_4_V  => link_out(4),
      link_out_5_V  => link_out(5),
      link_out_6_V  => link_out(6),
      link_out_7_V  => link_out(7),
      link_out_8_V  => link_out(8),
      link_out_9_V  => link_out(9),
      link_out_10_V => link_out(10),
      link_out_11_V => link_out(11),
      link_out_12_V => link_out(12),
      link_out_13_V => link_out(13),
      link_out_14_V => link_out(14),
      link_out_15_V => link_out(15),
      link_out_16_V => link_out(16),
      link_out_17_V => link_out(17),
      link_out_18_V => link_out(18),
      link_out_19_V => link_out(19),
      link_out_20_V => link_out(20),
      link_out_21_V => link_out(21),
      link_out_22_V => link_out(22),
      link_out_23_V => link_out(23),
      link_out_24_V => link_out(24),
      link_out_25_V => link_out(25),
      link_out_26_V => link_out(26),
      link_out_27_V => link_out(27),
      link_out_28_V => link_out(28),
      link_out_29_V => link_out(29),
      link_out_30_V => link_out(30),
      link_out_31_V => link_out(31),
      link_out_32_V => link_out(32),
      link_out_33_V => link_out(33),
      link_out_34_V => link_out(34),
      link_out_35_V => link_out(35),
      link_out_36_V => link_out(36),
      link_out_37_V => link_out(37),
      link_out_38_V => link_out(38),
      link_out_39_V => link_out(39),
      link_out_40_V => link_out(40),
      link_out_41_V => link_out(41),
      link_out_42_V => link_out(42),
      link_out_43_V => link_out(43),
      link_out_44_V => link_out(44),
      link_out_45_V => link_out(45),
      link_out_46_V => link_out(46),
      link_out_47_V => link_out(47),


      link_out_0_V_ap_vld  => link_out_ap_vld(0),
      link_out_1_V_ap_vld  => link_out_ap_vld(1),
      link_out_2_V_ap_vld  => link_out_ap_vld(2),
      link_out_3_V_ap_vld  => link_out_ap_vld(3),
      link_out_4_V_ap_vld  => link_out_ap_vld(4),
      link_out_5_V_ap_vld  => link_out_ap_vld(5),
      link_out_6_V_ap_vld  => link_out_ap_vld(6),
      link_out_7_V_ap_vld  => link_out_ap_vld(7),
      link_out_8_V_ap_vld  => link_out_ap_vld(8),
      link_out_9_V_ap_vld  => link_out_ap_vld(9),
      link_out_10_V_ap_vld => link_out_ap_vld(10),
      link_out_11_V_ap_vld => link_out_ap_vld(11),
      link_out_12_V_ap_vld => link_out_ap_vld(12),
      link_out_13_V_ap_vld => link_out_ap_vld(13),
      link_out_14_V_ap_vld => link_out_ap_vld(14),
      link_out_15_V_ap_vld => link_out_ap_vld(15),
      link_out_16_V_ap_vld => link_out_ap_vld(16),
      link_out_17_V_ap_vld => link_out_ap_vld(17),
      link_out_18_V_ap_vld => link_out_ap_vld(18),
      link_out_19_V_ap_vld => link_out_ap_vld(19),
      link_out_20_V_ap_vld => link_out_ap_vld(20),
      link_out_21_V_ap_vld => link_out_ap_vld(21),
      link_out_22_V_ap_vld => link_out_ap_vld(22),
      link_out_23_V_ap_vld => link_out_ap_vld(23),
      link_out_24_V_ap_vld => link_out_ap_vld(24),
      link_out_25_V_ap_vld => link_out_ap_vld(25),
      link_out_26_V_ap_vld => link_out_ap_vld(26),
      link_out_27_V_ap_vld => link_out_ap_vld(27),
      link_out_28_V_ap_vld => link_out_ap_vld(28),
      link_out_29_V_ap_vld => link_out_ap_vld(29),
      link_out_30_V_ap_vld => link_out_ap_vld(30),
      link_out_31_V_ap_vld => link_out_ap_vld(31),
      link_out_32_V_ap_vld => link_out_ap_vld(32),
      link_out_33_V_ap_vld => link_out_ap_vld(33),
      link_out_34_V_ap_vld => link_out_ap_vld(34),
      link_out_35_V_ap_vld => link_out_ap_vld(35),
      link_out_36_V_ap_vld => link_out_ap_vld(36),
      link_out_37_V_ap_vld => link_out_ap_vld(37),
      link_out_38_V_ap_vld => link_out_ap_vld(38),
      link_out_39_V_ap_vld => link_out_ap_vld(39),
      link_out_40_V_ap_vld => link_out_ap_vld(40),
      link_out_41_V_ap_vld => link_out_ap_vld(41),
      link_out_42_V_ap_vld => link_out_ap_vld(42),
      link_out_43_V_ap_vld => link_out_ap_vld(43),
      link_out_44_V_ap_vld => link_out_ap_vld(44),
      link_out_45_V_ap_vld => link_out_ap_vld(45),
      link_out_46_V_ap_vld => link_out_ap_vld(46),
      link_out_47_V_ap_vld => link_out_ap_vld(47)

      );

end architecture rtl;

