


#include "KAI/KAI.h"
#include "KAI/InstanceMap.h"

KAI_BEGIN

InstanceMapInfinite::InstanceMapInfinite(int num_buckets) : buckets(num_buckets)
{
}

void InstanceMapInfinite::SetNumBuckets(int num_buckets)
{
	throw;
}

void InstanceMapInfinite::insert(value_type const &value)
{
	if (!value.second)
		return;
	Bucket &bucket = GetBucket(value.first);
	bucket.push_back(value);
}

bool InstanceMapInfinite::Contains(Handle const &handle) const
{
	return find(handle) != end();
}

InstanceMapInfinite::Bucket &InstanceMapInfinite::GetBucket(Handle const &handle)
{
	return buckets[handle.GetValue() % buckets.size()];
}

InstanceMapInfinite::Bucket const &InstanceMapInfinite::GetBucket(Handle const &handle) const
{
	return buckets.at(handle.GetValue() % buckets.size());
}

InstanceMapInfinite::const_iterator InstanceMapInfinite::find(Handle const &handle) const
{
	Bucket const &bucket = GetBucket(handle);
	Bucket::const_iterator object = bucket.begin(), end = bucket.end();
	for (; object != end; ++object)
	{
		if (object->first == handle)
			return const_iterator();//object->second;
	}
	return this->end();
}

void InstanceMapInfinite::erase(const_iterator const &iter)
{
	//Bucket &bucket = GetBucket(handle);
	//Bucket::iterator object = bucket.begin(), end = bucket.end();
	//for (; object != end; ++object)
	//{
	//	if (object->first == handle)
	//	{
	//		bucket.eraes(object);
	//		return;
	//	}
	//}
}

//-------------------------------------------------------------------------

InstanceMapFinite::InstanceMapFinite(int num_buckets) : instances(num_buckets)
{
	clear();
}

InstanceMapFinite::const_iterator InstanceMapFinite::begin() const
{
	return instances.begin();
}

InstanceMapFinite::const_iterator InstanceMapFinite::end() const
{
	return instances.end();
}

void InstanceMapFinite::clear()
{
	nstd::fill(instances.begin(), instances.end(), value_type(0, 0));
}

bool InstanceMapFinite::empty() const
{
	throw;
}

void InstanceMapFinite::insert(value_type const &value)
{
	StorageBase *&ptr = instances[value.first.GetValue()];
	if (ptr != 0)
		KAI_THROW_0(ObjectExists);
	ptr = value.second;
}

InstanceMapFinite::const_iterator InstanceMapFinite::find(key_type const &key) const
{
	return instances[key.GetValue()];
}

void InstanceMapFinite::erase(key_type const &key)
{
	int num = key.GetValue();
	if (num >= (int)instances.size())
		KAI_THROW_0(InstancesExhausted);
	instances[num] = 0;
}

void InstanceMapFinite::erase(iterator const &iter)
{
	*iter = 0;
}

void InstanceMapFinite::SetNumBuckets(int)
{
	throw;
}

bool InstanceMapFinite::Contains(key_type const &key) const
{
	int num = key.GetValue();
	if (num >= (int)instances.size())
		KAI_THROW_0(InstancesExhausted);
	return instances[num] != 0;
}

KAI_END

//EOF
