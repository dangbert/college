#include <iostream>
#include <stdlib.h>
#include "Country.h"

string Country::getName() const {
	return name;
}
long Country::getPopulation() const {
	return population;
}
float Country::getLiteracyRate() const {
	return literacyRate;
}
float Country::getPrimaryCompletionFemale() const {
	return primaryCompletionFemale;
}
float Country::getPrimaryCompletionMale() const {
	return primaryCompletionMale;
}
float Country::getPrimaryCompletionTotal() const {
	return primaryCompletionTotal;
}
float Country::getEducationGDPSpent() const {
	return educationGDPSpent;
}
float Country::getYouthLitRateFem() const {
	return youthLitRateFem;
}
float Country::getYouthLitRateMale() const {
	return youthLitRateMale;
}


void Country::setName(string val) {
	name = val;
}
void Country::setPopulation(long val) {
	population = val;
}
void Country::setLiteracyRate(float val) {
	literacyRate = val;
}
void Country::setPrimaryCompletionFemale(float val) {
	primaryCompletionFemale = val;
}
void Country::setPrimaryCompletionMale(float val) {
	primaryCompletionMale = val;
}
void Country::setPrimaryCompletionTotal(float val) {
	primaryCompletionTotal = val;
}
void Country::setEducationGDPSpent(float val) {
	educationGDPSpent = val;
}
void Country::setYouthLitRateFem(float val) {
	youthLitRateFem = val;
}
void Country::setYouthLitRateMale(float val) {
	youthLitRateMale = val;
}
