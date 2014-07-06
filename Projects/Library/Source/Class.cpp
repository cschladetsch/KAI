
ClassBase::~ClassBase()
{
	Methods::iterator A = methods.begin(), B = methods.end();
	for (; A != B; ++A)
		delete A->second;
	Properties::iterator C = properties.begin(), D = properties.end();
	for (; C != D; ++C)
		delete C->second;
}

void ClassBase::SetMarked(StorageBase &Q, bool M) const
{
	Properties::const_iterator A = properties.begin(), B = properties.end();
	for (; A != B; ++A)
	{
		A->second->SetMarked(Q, M);
	}
	SetMarked2(Q, M);
}

