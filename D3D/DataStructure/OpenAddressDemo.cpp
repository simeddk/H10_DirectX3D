#include "stdafx.h"
#include "OpenAddressDemo.h"
#include "OpenAddress.h"

void OpenAddressDemo::Initialize()
{
	OpenAddress<const char*> hash(10);

	hash.Set("GCP", "Com2us");
	hash.Set("REDH", "Redhat");
	hash.Set("APAC", "Apache");
	hash.Set("ZYMZ", "Unisys");
	hash.Set("MSFT", "Microsoft");
	hash.Set("IBM", "IBM");
	hash.Set("ORCL", "Oracle");
	hash.Set("GOOG", "Google");
	hash.Set("YHOO", "Yahoo");
	hash.Set("NOVL", "Novell");

	string value = hash.Get("ZYMZ", 4);
	cout << value << endl;

	value = hash.Get("GCP", 4);
	cout << value << endl;
}


