#include <stdio.h>

#include <unistd.h>

#include <string.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#define env(s) #s
#define strEnv(s) env(s)

#define macroEqual(s1, s2) (strcmp(s1, s2))



/*
#ifndef compositor
#elif macroEqual(strEnv(compositor),"x11")
    #include "x/x.h"
#elif macroEqual(strEnv(compositor),"wayland")
#endif
*/


#ifdef compositor
#define isX macroEqual(strEnv(compositor), "x11")
#define isWayland macroEqual(strEnv(compositor), "wayland")
#ifdef isX
    #include "x/x.h"
#elif isWayland
#endif /* which compositor*/
#else
#endif

int main(int argc, char *argv[]) {
    struct leaseManager *lm = (struct leaseManager *)malloc(sizeof(struct leaseManager));
    int fd = createLease(lm);
    if (fd < 0) {
        printf("failed to lease resources: %d\n", fd);
        free(lm);
        return -1;
    }
    printf("leased resources\n");
    
    close(fd);
    revokeLease(lm);
    free(lm);
    return 0;
}
