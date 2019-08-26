from ROOT import *
from sys import argv
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d","--debug",help="enable debug options",action="store_true",default=False)
options,args = parser.parse_args()

nch = 48 # number of link channels 
bpl = 192 # bits per link
tpl = 18 # towers per link
bpt = 10 # bits per tower
maxTowers = 816
etunit = 4 # unit / GeV

def getExpectedTower(iphi,ieta):
  index = 136*( (iphi-1)/4 ) + (iphi-1)%4 + (ieta-1)*4;
  channel = index/18;
  bit = index%18;
  return index,channel,bit

def storeEt(et):
    maxEt = 2**bpt
    et = int(et * etunit)
    if et > maxEt: et = maxEt
    return str(bin(et)).replace('0b','').zfill(bpt)
    
def getTowerEt(i,event):
    iphi = i%4 + 4*(i/136) + 1
    ieta = (i/4)%34 + 1
    et = event.GetBinContent(iphi,ieta)
    return storeEt(et),et,iphi,ieta

def write(fname,links):
      with open('data/'+fname,'w') as out:
        out.write("=====================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================\n")
        out.write("WordCnt             LINK_00               LINK_01               LINK_02               LINK_03               LINK_04               LINK_05               LINK_06               LINK_07               LINK_08               LINK_09               LINK_10               LINK_11               LINK_12               LINK_13               LINK_14               LINK_15               LINK_16               LINK_17               LINK_18               LINK_19               LINK_20               LINK_21               LINK_22               LINK_23               LINK_24               LINK_25               LINK_26               LINK_27               LINK_28               LINK_29               LINK_30               LINK_31               LINK_32               LINK_33               LINK_34               LINK_35               LINK_36               LINK_37               LINK_38               LINK_39               LINK_40               LINK_41               LINK_42               LINK_43               LINK_44               LINK_45               LINK_46               LINK_47\n")
        out.write("#BeginData\n")
        for cyc in range(3):
          num = str((hex(cyc))[2:].zfill(4))
          out.write("0x%s   " % num )
          for i,link in enumerate(links):
            bitLo = len(link); bitHi = bitLo - 64
            save = link[bitHi:bitLo]; links[i] = link[:bitHi]
            num = str((hex( int(save,2) )).replace("L","")[2:].zfill(16))
            out.write("0x%s    " % num )
          out.write("\n")

input = args[-1]
tvfile = TFile.Open(input)
tvfile.cd('outputTV')

output = 'jetalgo_tv_'

events = [ tvfile.Get('outputTV/'+key.GetName()) for key in gDirectory.GetListOfKeys() ]
for i,event in enumerate(events):
    if options.debug:
        gStyle.SetTextSize(0.04)
        calo = TH2D("debug","Calorimeter",72,0,72,34,0,34)
    calo_towers = 0
    for section in range(3):
      links = []; ntowers = []
      link = ''; ntower = 0; total_towers = 0
      for j in range(calo_towers,calo_towers+maxTowers):
        if total_towers == maxTowers: break
        if len(links) == nch: break
        et,tower_et,iphi,ieta = getTowerEt(j,event)
        link = et + link; ntower += 1; total_towers += 1; calo_towers += 1
        if options.debug:
          print getTowerEt(j,event),len(links),ntower,total_towers
          calo.Fill(iphi-0.5,ieta-0.5,int(et,2))
        if ntower == tpl:
          link = link.zfill(bpl)
          links.append(link)
          ntowers.append(ntower)
          link = ''; ntower = 0
      while (len(links) != nch): links.append( ''.zfill(bpl) )
      if options.debug:
        avg_towers = sum( ntowers )/len(links)
        print "num channels %i | num of towers per channel %i" % ( len(links),avg_towers  )
        calo.Draw("COL TEXT")
        h = raw_input("Press any key to continue. ")
        continue
      write(output+'%i_%i_inp.txt' % (i,section),links)
        
###############################################
    
