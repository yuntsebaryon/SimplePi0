import sys
from ROOT import gSystem
gSystem.Load("libLArLiteApp_SimplePi0")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing SimplePi0..."

sys.exit(0)

