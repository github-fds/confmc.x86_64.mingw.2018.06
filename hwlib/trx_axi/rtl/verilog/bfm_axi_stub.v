module bfm_axi
(
       input   wire          SYS_CLK_STABLE
     , input   wire          SYS_CLK   // master clock and goes to SL_PCLK
     , output  wire          SYS_RST_N // by-pass of SL_RST_N
     , input   wire          SL_RST_N
     , output  wire          SL_CS_N   
     , output  wire          SL_PCLK   // by-pass of SYS_CLK after phase shift
     , output  wire  [ 1:0]  SL_AD
     , input   wire          SL_FLAGA // active-low empty (U2F)
     , input   wire          SL_FLAGB // active-low almost-empty
     , input   wire          SL_FLAGC // active-low full (F2U)
     , input   wire          SL_FLAGD // active-low almost-full
     , output  wire          SL_RD_N
     , output  wire          SL_WR_N 
     , output  wire          SL_OE_N // when low, let FX3 drive data through SL_DT_I
     , output  wire          SL_PKTEND_N
     , input   wire  [31:0]  SL_DT_I
     , output  wire  [31:0]  SL_DT_O
     , output  wire          SL_DT_T
     , input   wire  [ 1:0]  SL_MODE
     //-------------------------------------------------------------------------
     , input  wire                     ARESETn
     , input  wire                     ACLK
     , input  wire [ 3:0]              MID
     //-------------------------------------------------------------------------
     , output wire [ 3:0]              AWID
     , output wire [31:0]              AWADDR
     `ifdef AMBA_AXI4
     , output wire [ 7:0]              AWLEN
     , output wire                     AWLOCK
     `else
     , output wire [ 3:0]              AWLEN
     , output wire [ 1:0]              AWLOCK
     `endif
     , output wire [ 2:0]              AWSIZE
     , output wire [ 1:0]              AWBURST
     `ifdef AMBA_AXI_CACHE
     , output wire [ 3:0]              AWCACHE
     `endif
     `ifdef AMBA_AXI_PROT
     , output wire [ 2:0]              AWPROT
     `endif
     , output wire                     AWVALID
     , input  wire                     AWREADY
     `ifdef AMBA_AXI4
     , output wire [ 3:0]              AWQOS
     , output wire [ 3:0]              AWREGION
     `endif
     //-------------------------------------------------------------------------
     , output wire [ 3:0]              WID
     , output wire [31:0]              WDATA
     , output wire [ 3:0]              WSTRB
     , output wire                     WLAST
     , output wire                     WVALID
     , input  wire                     WREADY
     //-------------------------------------------------------------------------
     , input  wire [ 3:0]  BID
     , input  wire [ 1:0]              BRESP
     , input  wire                     BVALID
     , output wire                     BREADY
     //-------------------------------------------------------------------------
     , output wire [ 3:0]              ARID
     , output wire [31:0]              ARADDR
     `ifdef AMBA_AXI4
     , output wire [ 7:0]              ARLEN
     , output wire                     ARLOCK
     `else
     , output wire [ 3:0]              ARLEN
     , output wire [ 1:0]              ARLOCK
     `endif
     , output wire [ 2:0]              ARSIZE
     , output wire [ 1:0]              ARBURST
     `ifdef AMBA_AXI_CACHE
     , output wire [ 3:0]              ARCACHE
     `endif
     `ifdef AMBA_AXI_PROT
     , output wire [ 2:0]              ARPROT
     `endif
     , output wire                     ARVALID
     , input  wire                     ARREADY
     `ifdef AMBA_AXI4
     , output wire [ 3:0]              ARQOS
     , output wire [ 3:0]              ARREGION
     `endif
     //-------------------------------------------------------------------------
     , input  wire [ 3:0]              RID
     , input  wire [31:0]              RDATA
     , input  wire [ 1:0]              RRESP
     , input  wire                     RLAST
     , input  wire                     RVALID
     , output wire                     RREADY
     //-------------------------------------------------------------------------
     , input  wire                     IRQ
     , input  wire                     FIQ
     , output wire [ 15:0]             GPOUT
     , input  wire [ 15:0]             GPIN
);
// synthesis attribute box_type bfm_axi "black_box"
endmodule
