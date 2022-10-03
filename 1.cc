/**
 * File              : 1.cc
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.10.2022
 * Last Modified Date: 04.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>

extern "C" char* strptime(const char* s,
                          const char* f,
                          struct tm* tm) {
  // Isn't the C++ standard lib nice? std::get_time is defined such that its
  // format parameters are the exact same as strptime. Of course, we have to
  // create a string stream first, and imbue it with the current C locale, and
  // we also have to make sure we return the right things if it fails, or
  // if it succeeds, but this is still far simpler an implementation than any
  // of the versions in any of the C standard libraries.
  std::istringstream input(s);
  input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
  input >> std::get_time(tm, f);
  if (input.fail()) {
    return nullptr;
  }
  return (char*)(s + input.tellg());
}

int main(int argc, char *argv[])
{
	time_t sec = time(NULL);
	struct tm *tm = localtime(&sec);
	char s[BUFSIZ];
	strptime(s, "%Y-%m-%d %H:%M", tm);
	return 0;
}
