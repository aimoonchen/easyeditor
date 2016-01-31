#ifndef _LIBCOMPLEX_FILE_STORER_H_
#define _LIBCOMPLEX_FILE_STORER_H_



namespace ecomplex
{

class Symbol;

class FileStorer
{
public:
	static void Store(const char* filepath, const Symbol* symbol);
	static void StoreWithHistory(const char* filepath, const Symbol* symbol);

private:
	static void CenterSymbol(Symbol* symbol);
	static Json::Value Store(ee::Sprite* sprite, const wxString& dir);

}; // FileStorer

}

#endif // _LIBCOMPLEX_FILE_STORER_H_