#ifndef COUNTRY_H
#define COUNTRY_H

using namespace std;

class Country {
	public:
		string getName() const;
		long getPopulation() const;
		float getLiteracyRate() const;
		float getPrimaryCompletionFemale() const;
		float getPrimaryCompletionMale() const;
		float getPrimaryCompletionTotal() const;
		float getEducationGDPSpent() const;
		float getYouthLitRateFem() const;
		float getYouthLitRateMale() const;

		void setName(string val);
		void setPopulation(long val);
		void setLiteracyRate(float val);
		void setPrimaryCompletionFemale(float val);
		void setPrimaryCompletionMale(float val);
		void setPrimaryCompletionTotal(float val);
		void setEducationGDPSpent(float val);
		void setYouthLitRateFem(float val);
		void setYouthLitRateMale(float val);

	private:
		string name;
		long population;
		float literacyRate;
		float primaryCompletionFemale;
		float primaryCompletionMale;
		float primaryCompletionTotal;
		float educationGDPSpent;
		float youthLitRateFem;
		float youthLitRateMale;
};

#endif
