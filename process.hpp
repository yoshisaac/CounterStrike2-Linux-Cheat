#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <dirent.h>

#include "file.hpp"

namespace Process {

  static pid_t name_to_pid(std::string proc_name) {
    if (proc_name.length() == 0)
      return 0;

    //Try to open the /proc/ directory
    DIR *pDir = opendir("/proc/");

    if (pDir == nullptr) { return -1; } // if that fails then exit

    dirent* pDirent = nullptr;

    //basically loop through all the processes
    while (pDirent = readdir(pDir), pDirent != nullptr) {

      //Skip non processes
      if (pDirent->d_type != DT_DIR) {
	continue;
      }

      //Basically contains the process ID
      std::string str_proc_id = pDirent->d_name;

      //Get the name of the processes
      std::string str_comm = File::str_read("/proc/" + str_proc_id + "/comm");


      //Check if the currently iterated processes is the one we want
      if (str_comm == proc_name) {
	pid_t pid = (pid_t) atoi(str_proc_id.c_str());
	return pid;
      }
    }

    return 0;
  }

}
#endif
