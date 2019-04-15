void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == nullptr) {
		return;
	}
	string p = path;

	vector<MenuItem*>::const_iterator it;
	for (it = m->menuItems()->begin(); it != m->menuItems()->end(); it++) {
		path += "/" + (*it)->name();
		cout << path.substr(1) << endl;
		if ((*it)->menuItems() != nullptr) {
			listAll(*it, path);
		}
		path = p;
	}
}