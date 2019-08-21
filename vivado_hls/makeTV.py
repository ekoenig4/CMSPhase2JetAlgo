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

input = args[-1]
tvfile = TFile.Open(input)

output = 'jetalgo_tv_'

events = [ tvfile.Get(key.GetName()) for key in tvfile.GetListOfKeys() ]
for i,event in enumerate(events):
    if options.debug:
        gStyle.SetTextSize(0.04)
        calo = TH2D("debug","Calorimeter",72,0,72,34,0,34)
    links = []; ntowers = []
    link = ''; ntower = 0; total_towers = 0
    for j in range(2448):
        if total_towers == maxTowers: break
        if len(links) == nch: break
        et,tower_et,iphi,ieta = getTowerEt(j,event)
        link = et + link; ntower += 1; total_towers += 1
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
    with open('data/'+output+'%i_inp.txt' % i,'w') as out:
        for link in links:
            out.write(link+'\n')
###############################################
    
