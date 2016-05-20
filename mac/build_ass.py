import sys
from larlite import larlite as fmwk
from ROOT import TChain

#
# This script does:
# 0) read-in 2 input larlite ass_pandoraNu_tree from separate files
# 1) merge those associations stored separately and store in a new larlite output file
#

# Prepare storage_manager to make a new larlite output file
io = fmwk.storage_manager()
io.set_io_mode(io.kWRITE)
io.set_out_filename("out.root")
io.open()

# Prepare TChain for reco2d association 
ass_ch1 = TChain("ass_pandoraNu_tree")
for x in sys.argv:

    if 'larlite_reco2d' in x and x.endswith('.root'): ass_ch1.AddFile(x)

entries_2d = ass_ch1.GetEntries()
print 'Reco2D entries:',entries_2d

# Prepare TChain for reco3d association
ass_ch2 = TChain("ass_pandoraNu_tree")
for x in sys.argv:

    if 'larlite_reco3d_pandora' in x and x.endswith('.root'): ass_ch2.AddFile(x)
entries_3d = ass_ch2.GetEntries()
print 'Reco3D entries:',entries_3d

# Get entries to loop over: minimum of 2 TTrees' entries 
entries = entries_2d
if entries_3d < entries: 
    print '\033[95m[WARNING]\033[00m # entries in 2d (%d) vs. 3d (%d) do not match!' % (entries_2d,entries_3d)
    entries = entries_3d

# Loop over entries and make an output larlite file
for entry in xrange(entries):

    # Read-in TTree entry
    ass_ch1.GetEntry(entry)
    ass_ch2.GetEntry(entry)

    # Get larlite::event_ass pointers
    ass_2d = ass_ch1.ass_pandoraNu_branch
    ass_3d = ass_ch2.ass_pandoraNu_branch

    # Check event alignment
    if (not ass_2d.run() == ass_3d.run() or
        not ass_2d.subrun() == ass_3d.subrun() or
        not ass_2d.event_id() == ass_3d.event_id() ):
        print '\033[95mERROR\033[00m Event mis-alignment detected!'
        break

    # Get output larlite::event_ass handle
    ev_ass = io.get_data(fmwk.data.kAssociation,"pandoraNu")

    pairs = []
    
    # Loop over reco2d association from TTree and insert them into output
    for ass_id in xrange(ass_2d.size()):

        ass_key_pair = ass_2d.association_keys(ass_id)

        key_type1 = int( ass_key_pair.first.first   )
        key_name1 = str( ass_key_pair.first.second  )
        key_type2 = int( ass_key_pair.second.first  )
        key_name2 = str( ass_key_pair.second.second )

        key_id = None

        if key_type1 < key_type2:
            key_id = (key_type1,key_type2,key_name1,key_name2)
        else:
            key_id = (key_type2,key_type1,key_name2,key_name1)            

        if key_id in pairs: continue

        ev_ass.set_association(ass_key_pair.first,
                               ass_key_pair.second,
                               ass_2d.association(ass_id))

        pairs.append(key_id)

    #ev_ass.list_association()
    
    # Loop over reco3d association from TTree and insert them into output
    for ass_id in xrange(ass_3d.size()):

        ass_key_pair = ass_3d.association_keys(ass_id)

        key_type1 = int( ass_key_pair.first.first   )
        key_name1 = str( ass_key_pair.first.second  )
        key_type2 = int( ass_key_pair.second.first  )
        key_name2 = str( ass_key_pair.second.second )

        key_id = None

        if key_type1 < key_type2:
            key_id = (key_type1,key_type2,key_name1,key_name2)
        else:
            key_id = (key_type2,key_type1,key_name2,key_name1)            

        if key_id in pairs: continue
        
        ev_ass.set_association(ass_key_pair.first,
                               ass_key_pair.second,
                               ass_3d.association(ass_id))
        ass = ass_3d.association(ass_id)
        pairs.append(key_id)
        
#        if ( (key_id[0] == fmwk.data.kPFParticle and key_id[1] == fmwk.data.kCluster) or
#             (key_id[0] == fmwk.data.kCluster and key_id[1] == fmwk.data.kPFParticle) ):
#            
#            print key_id
#            for x in xrange(ass.size()):
#                print x,'=>',ass[x].size(),' objects...'

    #ev_ass.list_association()
    # Set event id
    io.set_id( ass_2d.run(), ass_2d.subrun(), ass_2d.event_id() )

    # Store
    io.next_event()

# Close file
io.close()
    




