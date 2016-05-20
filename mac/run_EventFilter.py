import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

evtfilter = fmwk.EventFilter()
evtfilter.SetInputProducer( "pandoraNu" )
evtfilter.AddAnEvent( 6058, 177, 8877 )
evtfilter.AddAnEvent( 6145, 16, 814 )
evtfilter.AddAnEvent( 5975, 85, 4262 )
evtfilter.AddAnEvent( 6058, 94, 4706 )
evtfilter.AddAnEvent( 6015, 69, 3465 )
evtfilter.AddAnEvent( 6070, 113, 5680 )

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify output root file name
my_proc.set_ana_output_file("larlite_filtered_ana.root");

my_proc.set_output_file("larlite_filtered.root");

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_proc.enable_filter( True )
my_proc.add_process( evtfilter )

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
