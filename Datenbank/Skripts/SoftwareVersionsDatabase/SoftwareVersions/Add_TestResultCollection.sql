ALTER TABLE TestCollectionResult
ADD TestCollection_Id int references TestCollection(Id) NOT NULL;