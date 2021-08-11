
void listAll(const File* f, string path)  // two-parameter overload
{
    if (f->files() == nullptr)
    {
        cout << path + "/" + f->name() << "\n";
        return;
    }
    string toPrint = path + "/" + f->name();
    cout << toPrint << "\n";
    vector<File*>::const_iterator it = (f->files())->begin();
    while (it != (f->files())->end())
    {
        listAll((*it), toPrint);
        it++;
    }
}
