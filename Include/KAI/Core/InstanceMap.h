

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_INSTANCE_MAP_H
#	define KAI_INSTANCE_MAP_H

KAI_BEGIN

/// Common base for all InstanceMap implementations
struct InstanceMapBase
{
	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;

	virtual void clear() = 0;
	virtual bool empty() const = 0;
	virtual void insert(value_type const &) = 0;
	virtual const_iterator find(key_type const &) const = 0;
	virtual void erase(key_type const &) = 0;
	virtual void erase(const_iterator const &) = 0;

	virtual void SetNumBuckets(int) = 0;
	virtual bool Contains(key_type const &) const = 0;
};

/// An instance map that can store any number of instances
struct InstanceMapInfinite : InstanceMapBase
{
	typedef Handle key_type;
	typedef StorageBase *referent_type;
	typedef nstd::pair<const key_type, referent_type> value_type;

	typedef nstd::list<nstd::pair<Handle, StorageBase *> > Bucket;
	typedef nstd::vector<Bucket> Buckets;
	class const_iterator
	{
		Buckets::const_iterator bucket;
		Bucket::const_iterator object;

	public:
		const_iterator() { }
		const_iterator(Buckets::const_iterator B, Bucket::const_iterator C) : bucket(B), object(C) { }

		// postfix
		const_iterator operator++(int)
		{
			const_iterator tmp(*this);
			++*this;
			return tmp;
		}
		// prefix
		const_iterator operator++();

		value_type const *operator->() const;
		value_type const &operator*() const;

		friend bool operator==(const_iterator, const_iterator);
		friend bool operator!=(const_iterator const &A, const_iterator const &B) { return !(A == B); }
	};

private:
	Buckets buckets;

public:
	InstanceMapInfinite(int num_buckets = 42961);

	const_iterator begin() const;
	const_iterator end() const;

	void clear();
	bool empty() const;
	void insert(value_type const &);
	const_iterator find(key_type const &) const;
	void erase(key_type const &);
	void erase(const_iterator const &);

	void SetNumBuckets(int);
	bool Contains(key_type const &) const;

protected:
	Bucket &GetBucket(Handle const &);
	Bucket const &GetBucket(Handle const &) const;
};

/// A finite instance map
struct InstanceMapFinite : InstanceMapBase
{
	typedef nstd::pair<Handle, StorageBase *> value_type;
	typedef nstd::vector<value_type> Instances;
	typedef Values::const_iterator const_iterator;

private:
	Instances instances;

public:
	InstanceMapFinite(int num_buckets = 42961);

	const_iterator begin() const;
	const_iterator end() const;

	void clear();
	bool empty() const;
	void insert(value_type const &);
	const_iterator find(key_type const &) const;
	void erase(key_type const &);
	void erase(iterator const &);

	void SetNumBuckets(int);
	bool Contains(key_type const &) const;
};

KAI_END

#endif // KAI_INSTANCE_MAP_H

//EOF
