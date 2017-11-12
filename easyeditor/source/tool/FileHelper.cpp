#include "FileHelper.h"
#include "StringHelper.h"
#include "Config.h"
#include "SymbolFile.h"

#include <wx/dir.h>
#include <wx/filename.h>
#ifndef _DEBUG
#include <wx/stdpaths.h>
#endif // _DEBUG

#include <set>

namespace ee
{

bool FileHelper::MkDir(const std::string& dir, bool rm)
{
	bool ret = true;
	if (wxDir::Exists(dir.c_str())) {
		if (rm) {
			wxFileName::Rmdir(dir.c_str(), wxPATH_RMDIR_RECURSIVE);
			ret = wxFileName::Mkdir(dir.c_str(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}
	} else {
		ret = wxFileName::Mkdir(dir.c_str(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	return ret;
}

bool FileHelper::RmFile(const std::string& filepath)
{
	return wxRemoveFile(filepath.c_str());
}

bool FileHelper::IsFileExist(const std::string& filepath)
{
	return wxFileName::FileExists(filepath.c_str());
}

bool FileHelper::IsDirExist(const std::string& filepath)
{
	return wxFileName::DirExists(filepath.c_str());
}

void FileHelper::FetchAllFiles(const std::string& dirpath, wxArrayString& files)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files) 
			: m_files(files) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename) override
		{
			m_files.Add(filename);
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname) override
		{
			return wxDIR_CONTINUE;
		}

	private:
		wxArrayString& m_files;

	}; // DirTraverser

	DirTraverser traverser(files);

	wxDir dir(dirpath.c_str());
	dir.Traverse(traverser);
}

void FileHelper::FetchAllFiles(const std::string& dirpath, const std::vector<std::string>& ignore_dirs, wxArrayString& files)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files, const std::vector<std::string>& ignore_dirs) 
			: m_files(files)
		{
			for (int i = 0, n = ignore_dirs.size(); i < n; ++i) {
				m_ignore_dirs.insert(ignore_dirs[i]);
			}
		}

		virtual wxDirTraverseResult OnFile(const wxString& filename) override
		{
			m_files.Add(filename);
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname) override
		{
			if (m_ignore_dirs.find(dirname.ToStdString().c_str()) != m_ignore_dirs.end()) {
				return wxDIR_IGNORE;
			} else {
				return wxDIR_CONTINUE;
			}
		}

	private:
		wxArrayString& m_files;
		std::set<std::string> m_ignore_dirs;

	}; // DirTraverser

	DirTraverser traverser(files, ignore_dirs);

	wxDir dir(dirpath.c_str());
	dir.Traverse(traverser);
}

void FileHelper::FetchAllFiles(const std::string& dirpath, wxArrayString& files, int type)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files, int type) 
			: files(files), type(type) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename) override
		{
			if (SymbolFile::Instance()->Type(filename.ToStdString().c_str()) == type) {
				files.Add(filename);
			}
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname) override
		{
			return wxDIR_CONTINUE;
		}

	private:
		wxArrayString& files;
		int type;

	}; // DirTraverser

	DirTraverser traverser(files, type);

	wxDir dir(dirpath.c_str());
	dir.Traverse(traverser);
}

void FileHelper::FetchCurrDirs(const std::string& dirpath, wxArrayString& dirs)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files) 
			: m_files(files) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename) override
		{
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname) override
		{
			m_files.Add(dirname);
			return wxDIR_IGNORE;
		}

	private:
		wxArrayString& m_files;

	}; // DirTraverser

	DirTraverser traverser(dirs);

	wxDir dir(dirpath.c_str());
	dir.Traverse(traverser);
}

std::string FileHelper::GetFilenameAddTag(const std::string& filename, const std::string& tag, 
									   const std::string& extension)
{
	std::string fixed;
	int start = filename.find_last_of('_');
	if (start != -1)
	{
		std::string check = filename.substr(start + 1, filename.find_last_of('.') - start - 1);
		if (check == tag) {
			fixed = filename;
		} else if (filename[0] == '.') {
			fixed = filename + std::string("_" + tag + "." + extension);
		} else {
			fixed = filename.substr(0, filename.find_last_of('.')) + std::string("_" + tag + "." + extension);
		}
	}
	else
	{
		if (filename[0] == '.') {
			fixed = filename.substr(0, filename.find_last_of('.')) + std::string("_" + tag + "." + extension);
		} else {
			int dot_pos = filename.find_last_of('.');
			if (dot_pos == -1) {
				fixed = filename + std::string("_" + tag + "." + extension);
			} else {
				fixed = filename;
				fixed.insert(dot_pos, "_" + tag);
			}
		}
	}
	return fixed;
}

std::string FileHelper::GetFilenameTag(const std::string& filepath)
{
	const size_t start = filepath.find_last_of('_') + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

std::string FileHelper::GetFilename(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	const size_t start = pos_divide + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

std::string FileHelper::GetFilenameWithExtension(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	return filepath.substr(pos_divide + 1);
}

std::string FileHelper::GetRelativePath(const std::string& dir, const std::string& absolute)
{
	wxFileName filename(absolute.c_str());
	filename.MakeRelativeTo(dir.c_str());
	return filename.GetFullPath().Lower().ToStdString().c_str();
}

std::string FileHelper::GetAbsolutePath(const std::string& dir, const std::string& relative)
{
	wxFileName filename(relative.c_str());
	filename.MakeAbsolute(dir.c_str());
	filename.Normalize();
	return filename.GetFullPath().ToStdString().c_str();
}

std::string FileHelper::GetAbsolutePath(const std::string& filepath)
{
	wxFileName filename(filepath.c_str());
	filename.Normalize();
	return filename.GetFullPath().ToStdString().c_str();
}

std::string FileHelper::GetAbsolutePathFromFile(const std::string& base, const std::string& relative)
{
	std::string dir = FileHelper::GetFileDir(base);
	return FileHelper::GetAbsolutePath(dir, relative);
}

std::string FileHelper::GetFilePathExceptExtension(const std::string& filepath)
{
	return filepath.substr(0, filepath.find_last_of('.'));
}

std::string FileHelper::GetExtension(const std::string& filepath)
{
	return filepath.substr(filepath.find_last_of('.') + 1);
}

std::string FileHelper::GetFileDir(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	if (pos_divide == -1) {
		return ".";
	} else {
		return filepath.substr(0, pos_divide);
	}
}

std::string FileHelper::GetExistFilepath(const std::string& filepath, const std::string& dir /*= wxEmptyString*/)
{
	std::string fixed = filepath;
	if (!IsFileExist(fixed))
	{
		std::string filename = fixed = GetFilenameWithExtension(fixed);
		if (!IsFileExist(fixed))
		{
			std::string cwd = wxFileName::GetCwd();

			Config* cfg = Config::Instance();
			std::set<std::string>::const_iterator 
				itr = cfg->GetResPathes().begin();
			for ( ; itr != cfg->GetResPathes().end(); ++itr)
			{
				fixed = *itr + filename;
				if (IsFileExist(fixed))
					return fixed;

				fixed = cwd + *itr + filename;
				if (IsFileExist(fixed))
					return fixed;
			}
			if (!dir.empty())
			{
				fixed = dir + filename;
				if (IsFileExist(fixed))
					return fixed;
			}
// 			throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
			return "";
		}
		else
		{
			return fixed;
		}
	}
	else
	{
		return fixed;
	}
}

void FileHelper::FormatSeparators(std::string& filepath)
{
	const std::string oldVal = "/", newVal = "\\";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filepath.find(oldVal, pos)) != std::string::npos)
			filepath.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}   
}

std::string FileHelper::FormatFilepath(const std::string& filepath)
{
	std::string ret = filepath;
	StringHelper::ToLower(ret);
	FormatSeparators(ret);
	return ret;
}

std::string FileHelper::FormatFilepathAbsolute(const std::string& filepath)
{
	wxFileName filename(FormatFilepath(filepath).c_str());
	filename.Normalize();
	return filename.GetFullPath().Lower().ToStdString().c_str();
}

std::string FileHelper::GetJsonFileFilter(const std::string& file_tag)
{
	return StringHelper::Format("EASY files (*_%s.json)|*_%s.json", file_tag.c_str(), file_tag.c_str());
}

}