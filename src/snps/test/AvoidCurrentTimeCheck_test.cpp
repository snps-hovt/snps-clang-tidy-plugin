// snps-avoid-current-time, misc-definitions-in-headers
//

#include <time.h>
#include <ctime>
#include <sys/time.h>

int main()
{
    // warning: function 'time' is making the application non-deterministic.
    // warning: Using 'time_t' (aka 'long') could making the application non-deterministic.
    auto t = time(NULL);
    // warning: function 'time' is making the application non-deterministic.
    const std::time_t now = std::time(nullptr);

    struct timeval tv;
    struct timezone tz;
    // warning: function 'gettimeofday' is making the application non-deterministic.
    gettimeofday(&tv,&tz);
    // warning: Using 'time_t' (aka 'long') could making the application non-deterministic.
    time_t epoch = 0;
    return 0;
}