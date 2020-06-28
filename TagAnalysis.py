import os
import sys
import enum

class SerializeType(enum.Enum):
    STDOUT = enum.auto()
    EMACS  = enum.auto()


class Settings():
    serialize_type = SerializeType.STDOUT


class TagType(enum.Enum):
    TODO      = enum.auto()
    PERF      = enum.auto()
    NOTE      = enum.auto()
    TEMP      = enum.auto()
    TEST      = enum.auto()
    STUDY     = enum.auto()
    DEBUG     = enum.auto()
    REMOVE    = enum.auto()
    FINDME    = enum.auto()
    REVISIT   = enum.auto()
    RESTORE   = enum.auto()
    STEP_INTO = enum.auto()
    CLEAN_UP  = enum.auto()


def IsTagTypeEmacsErrorType(tag_type):
    isEmacsType = False
    if (tag_type == TagType.REMOVE) or (tag_type == TagType.STEP_INTO):
        isEmacsType = True

    return isEmacsType


RawTags = set(())
class Tag:
    def __init__(self, _file_name, _absolute_path, _line_number, _message, _type):
        self.file_name       = _file_name
        self.absolute_path   = _absolute_path
        self.line_number     = _line_number
        self.message         = _message
        self.type            = _type

    def __lt__(self, other):
        file_names = [ self.file_name, other.file_name ]
        file_names.sort()
        if file_names[0] == file_names[1]:
            line_numbers = [ self.line_number, other.line_number ]
            line_numbers.sort()
            if line_numbers[0] == self.line_number:
                return True
            else:
                return False
        elif file_names[0] == self.file_name:
            return True
        else:
            return False

    def Serialize(self, serialize_type):
        serialized_str = ""
        if serialize_type == SerializeType.STDOUT:
            serialized_str = '[ ' + self.type.name + ' ]' + ' (' + self.file_name + ', ' + str(self.line_number + 1) + '): ' + self.message
        elif serialize_type == SerializeType.EMACS:
            serialized_str = self.absolute_path + '(' + str(self.line_number + 1) + '): '
            if IsTagTypeEmacsErrorType(self.type):
                serialized_str = serialized_str + 'error: ' + self.message
            else:
                serialized_str = serialized_str + 'warning: ' + self.message

        return serialized_str


Sources = set(())
class SOURCE:
    def __init__(self, _root, _file_name):
        self.root          = _root
        self.file_name     = _file_name
        self.absolute_path = os.path.join(_root, _file_name)
        self.line_count    = 0

    def Analyze(self):
        with open(self.absolute_path) as file:
            for line_index, line_data in enumerate(file):
                for type in (TagType):
                    cf_prefix = 'cfarvin'
                    code_tag = '[ ' + cf_prefix + '::' + type.name + ' ]'
                    if (cf_prefix in line_data) and (type.name in line_data):
                        cleaned_line_data = line_data
                        cleaned_line_data = cleaned_line_data.replace('//', '')
                        cleaned_line_data = cleaned_line_data.replace('/*', '')
                        cleaned_line_data = cleaned_line_data.replace('*/', '')
                        cleaned_line_data = cleaned_line_data.replace(code_tag, '')
                        cleaned_line_data = cleaned_line_data.strip()
                        RawTags.add(Tag(self.file_name, self.absolute_path, line_index, cleaned_line_data, type))
                self.line_count = line_index + 1


ProcessedTags = list()
def ProcessTags():
    source_file_extensions = ('.c', '.C', '.h', '.H', '.cpp', '.CPP', '.cxx')
    for root, directories, file_names in os.walk(os.getcwd()):
        for file_name in file_names:
            for extension in source_file_extensions:
                if len(file_name) > len(extension):
                     if (file_name[-len(extension):] == extension):
                        Sources.add(SOURCE(root, file_name))

    for source_file in Sources:
        source_file.Analyze()

    for type in TagType:
        ProcessedTags.append(list())

    for tag in RawTags:
        ProcessedTags[tag.type.value - 1].append(tag)


def GenerateReport(serialize_type):
    report_strings = list()

    for tag_index, tag_list in enumerate(ProcessedTags):
        num_items = len(ProcessedTags[tag_index])
        if num_items > 0:
            tag_list.sort()
            header_str = 'There are ' + str(num_items) + ' items tagged ' + TagType(tag_index + 1).name + ':'
            report_strings.append(header_str)
            for tag in tag_list:
                report_strings.append('    ' + tag.Serialize(serialize_type))
            report_strings.append('\n')

    return '\n'.join(report_strings)


def FileReport(file_path):
    report_data = GenerateReport(SerializeType.STDOUT)
    if len(report_data) > 0:
        with open(file_path, 'w+') as file:
            file_header = '= This file is generated on compilation. Content changes will be overwritten ='
            file_header_separator = ('=' * len(file_header)) + '\n'
            file.write(file_header_separator)
            file.write(file_header + '\n')
            file.write(file_header_separator + '\n')
            file.write(report_data)
    elif os.path.exists(file_path):
        os.remove(file_path)


def ParseCommandArgs():
    settings = Settings()
    arg_list = str(sys.argv)
    for type in SerializeType:
        lowercase_type = type.name.lower()
        if type.name in arg_list or lowercase_type in arg_list:
            settings.serialize_type = type

    return settings

if __name__ == '__main__':
    GlobalSettings = ParseCommandArgs()
    ProcessTags()

    # Write file type report to disk
    FileReport(os.path.join(os.getcwd(), 'Tag Analysis Results.txt'))

    # Log report to SerializeType
    console_report = GenerateReport(GlobalSettings.serialize_type)
    if len(console_report) > 0:
        print(console_report)
    else:
        print('Tag analysis has nothing to report')
