library ieee;
use ieee.std_logic_1164.all;

package algo_pkg is

  type LinkMasterType is record
    tValid : std_logic;
    tData  : std_logic_vector(63 downto 0);
    tLast  : std_logic;
  end record LinkMasterType;

  type LinkMasterArrType is array (natural range<>) of LinkMasterType;

  type LinkSlaveType is record
    tReady : std_logic;
  end record LinkSlaveType;

  type LinkSlaveArrType is array (natural range<>) of LinkSlaveType;

component algo_top_wrapper
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

end component algo_top_wrapper;

end package;
