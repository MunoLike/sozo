/*
 * filelib.cpp
 *
 *  Created on: 2020/11/22
 *      Author: MunoLike
 */

#include <glob.h>
#include <string>

#include "fileutil.hpp"

//@param vague path (ex.)/sys/devices/ocp.*/pwm_test_P9_14.*/
/*
 * @return absolute path (ex.)/sys/devices/ocp.3/pwm_test_P9_14.15/
 * */
namespace utils {
std::string unclear_pathto_abs(std::string unclear_path) {
  glob_t globbuf;
  glob(unclear_path.c_str(), GLOB_MARK, NULL, &globbuf);

  std::string abs_path(globbuf.gl_pathv[0]);

  globfree(&globbuf);

  return abs_path;
}
}
