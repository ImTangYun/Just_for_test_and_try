//
//
//
#include "log.h"
#include "rsync_core.h"
#include "time_utils.h"

int main(int argc, char** argv)
{
    if (argc < 3) {
        WLOG(ERROR, "Usage: ./rdiff <old_file> <sig_file> <new_file> <delta_file> <result>");
        return -1;
    }
    TimeCounter time_counter;
    RsyncCore rsync;
    if (argc >= 3)
        rsync.GenerateSigFile(argv[1], argv[2]);
    time_counter.AddNow();
    WLOG(DEBUG, "sig cost %d", time_counter.GetTimeCosts(1));
    if (argc >= 5)
        rsync.GenerateDeltaFile(argv[3], argv[2], argv[4]);
    time_counter.AddNow();
    WLOG(DEBUG, "delta cost %d", (time_counter.GetTimeCosts(2) - time_counter.GetTimeCosts(1)));
    if (argc == 6)
        rsync.PatchFile(argv[1], argv[4], argv[5]);
    time_counter.AddNow();
    WLOG(DEBUG, "patch cost %d", (time_counter.GetTimeCosts(3) - time_counter.GetTimeCosts(2)));
    return 0;
}
