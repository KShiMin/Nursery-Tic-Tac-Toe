import argparse, os, platform, subprocess, sys, logging

class UnsupportedOSError(Exception):
    # Custom exception for unsupported or unknown operating system.
    pass

class Compiler: 
    def __init__(self, file: str=None, folder: str=None, output: str=None) -> None: 
        logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")
        self.logger = logging.getLogger(__name__)

        self.file = file 
        self.folder = folder 
        self.output = output
        self.__os_type = self._detect_os()
        self.__script_path = os.getcwd()



    def _detect_os(self) -> str: 
        os_name = platform.system()
        match os_name: 
            case "Linux": 
                # self.logger.info(f"Detected OS: {os_name}")
                return "Linux"

            case "Darwin":
                # self.logger.info(f"Detected OS: {os_name}")
                return "Mac"
                
            case "Windows" | "CYGWIN" | "MINGW":
                # self.logger.info(f"Detected OS: {os_name}")
                return "Windows"
                
            case _: 
                self.logger.error(f"Unsupported or unknown operating system: {os_name}")
                raise UnsupportedOSError(f"Unsupported or unknown operating system: {os_name}")
        
    def _normalise_path(self, path: str) -> str:
        if self.__os_type == "Windows":
            # check if `cygpath` exists before using it 
            cygpath_exists = shutil.which("cygpath") is not None
            if cygpath_exists: 
                result = subprocess.run(["cygpath", "-w", path], capture_output=True, text=True)
                normalised_path = result.stdout.strip()
                self.logger.debug(f"Normalised path for Windows using cygpath: {normalised_path}")
                return normalised_path
            else: 
                # if `cygpath` is not available, assume the path is already in Windows format
                self.logger.debug(f"Path does not require normalisation: {path}")
                return path
        
        # for other OS types, return the path as is
        self.logger.debug(f"Path does not require normalisation: {path}")
        return path
        
        # if self.__os_type == "Windows":
        #     result = subprocess.run(["cygpath", "-w", path], capture_output=True, text=True)
        #     normalised_path = result.stdout.strip()
        #     self.logger.debug(f"Normalised path for WIndows: {normalised_path}")
        #     return normalised_path
        
        # self.logger.debug(f"Path deos not require normalisation: {path}")
        # return path

    def compile_file(self) -> None: 
        filepath = self._normalise_path(os.path.join(self.__script_path, self.file))

        if not os.path.isfile(filepath):
            raise FileNotFoundError(f"Error: File '{self.file}' not found.")
            sys.exit(1)
        
        output_name = self.output if self.output else os.path.splitext(self.file)[0]
        self.logger.info(f"Compiling file {self.file}...")

        result = subprocess.run(["gcc", "-o", output_name, filepath])
        if result.returncode == 0: 
            self.logger.info("Compilation successful.")
            self._execute_output(output_name)
        else: 
            self.logger.error("Compilation failed.")
            sys.exit(1)
    
    def compile_folder(self) -> None: 
        folderpath = self._normalise_path(os.path.join(self.__script_path, self.folder))

        if not os.path.isdir(folderpath):
            self.logger.error(f"Folder '{self.folder}' not found.")
            sys.exit(1)

        self.logger.info(f"Compiling all C files in folder {self.folder}...")

        files = [os.path.join(folderpath, f) for f in os.listdir(folderpath) if f.endswith(".c")]
        if not files: 
            self.logger.warning(f"No C files found in folder {self.folder}.")
            sys.exit(1)

        output_name = self.output if self.output else os.path.splitext(os.path.basename(files[0]))[0]
        result = subprocess.run(["gcc", "-o", output_name, *files])

        if result.returncode == 0: 
            self.logger.info(f"Compilation successful: {output_name}")
            self._execute_output(output_name) 
        else: 
            self.logger.error("Compilation failed.")
            sys.exit(1)
        
    def _execute_output(self, output_name: str) -> None: 
        try: 
            self.logger.info(f"Executing output: {output_name}")
            if "Windows" in self.__os_type:
                subprocess.run([output_name], shell=True)
                input("Press enter to continue...")
            else: 
                subprocess.run([f"./{output_name}"])
        except Exception as e:
            self.logger.error(f"Execution failed: {e}")
            sys.exit(1)

    @staticmethod
    def display_help():
        help_str = """
        Usage: compile.py [-h] [-f <file>] [-F <folder>] [-o <output>]
        -h              Show this help message.
        -f  <file>      Compile a single C file.
        -F  <folder>    Compile all C files in the specified folder.
        -o  <output>    Specify the output executable name.
        """
        print(help_str)

def main():
    parser = argparse.ArgumentParser(description="Compile and execute C files.")
    parser.add_argument("-f", "--file", help="Compile a single C file.")
    parser.add_argument("-F", "--folder", help="Compile all C files in the specified folder.")
    parser.add_argument("-o", "--output", help="Specify the output executable name.")
    args = parser.parse_args()

    if args.file and args.folder:
        print("Error: You cannot specify both -f and -F options.")
        sys.exit(1)
    
    compiler = Compiler(file=args.file, folder=args.folder, output=args.output)

    if args.file: 
        compiler.compile_file()
    elif args.folder:
        compiler.compile_folder()
    else: 
        Compiler.display_help()

if __name__ == "__main__":
    main()