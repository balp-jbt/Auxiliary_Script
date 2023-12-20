import os


# Singleton in Python to debug
class DebugClass:
    _instance = None

    def __new__(cls, f_path=None):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self, f_path=None):
        self.blank_line = "\n"
        self.title_len = 70
        self.title_height = 2
        self.content_cnt = 0
        self.title_cnt = 0
        self.f = None if f_path is None else open(f_path, 'w')
    
    def __del__(self):
        if self.f is not None:
            self.f.close()

    def change_file(self, f_path):
        if self.f is not None:
            self.f.close()
        
    def cat_file(self):
        if self.f is not None:
            os.system("cat " + self.f.name())

    def alert_content(self, content):
        if self.f is None:
            print(str(self.content_cnt) + "\t" + content)
        else:
            self.f.write(str(self.content_cnt) + "\t" + content + "\n")
        self.content_cnt += 1

    def alert_title(self, content):
        space_left = (self.title_len - 2 - len(content)) // 2
        space_left = max(0, space_left)
        space_right = self.title_len - 2 - space_left - len(content)
        space_right = max(0, space_right)
        title_blank_line = "*" + " " * (self.title_len - 2) + "*"
        if self.f is None:
            print("*" * self.title_len)
            for _ in range(self.title_height):
                print(title_blank_line)
            print("*" + " " * space_left + content + " " * space_right + "*")
            for _ in range(self.title_height):
                print(title_blank_line)
            print("*" * self.title_len)
        else:
            self.f.write("*" * self.title_len + "\n")
            for _ in range(self.title_height):
                self.f.write(title_blank_line + "\n")
            self.f.write("*" + " " * space_left + content + " " * space_right + "*" + "\n")
            for _ in range(self.title_height):
                self.f.write(title_blank_line + "\n")
            self.f.write("*" * self.title_len + "\n")
    
    def begin_function(self, function_name):
        self.alert_content("Begin function " + function_name)
            
    def end_function(self, function_name):
        self.alert_content("End function " + function_name)


# Test for Singleton Mode
if __name__ == "__main__":
    class1 = DebugClass()
    class2 = DebugClass()
    print(class1 == class2)
    exit(0)