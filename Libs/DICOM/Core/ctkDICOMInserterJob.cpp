/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Davide Punzo, punzodavide@hotmail.it,
  and development was supported by the Center for Intelligent Image-guided Interventions (CI3).

=========================================================================*/

// ctkDICOMCore includes
#include "ctkDICOMInserterJob.h"
#include "ctkDICOMInserterWorker.h"
#include "ctkLogger.h"

static ctkLogger logger ("org.commontk.dicom.DICOMInserterJob");

//------------------------------------------------------------------------------
// ctkDICOMInserterJob methods

//------------------------------------------------------------------------------
ctkDICOMInserterJob::ctkDICOMInserterJob()
{
  this->MaximumConcurrentJobsPerType = 1;
}

//------------------------------------------------------------------------------
ctkDICOMInserterJob::~ctkDICOMInserterJob() = default;

//------------------------------------------------------------------------------
QString ctkDICOMInserterJob::loggerReport(const QString& status)
{
  QString fullLogMsg;
  QString logMsg;

  QString uids;
  foreach (QSharedPointer<ctkDICOMJobResponseSet> JobResponseSet, this->JobResponseSets)
  {
    uids += "job type : " + JobResponseSet->jobTypeString() + ": \n" ;
    uids += JobResponseSet->datasets().keys().join(", ") + ": \n";
  }

  fullLogMsg = QString("ctkDICOMInserterJob: insert job %1. "
                       "Number of jobResponseSet processing: %2.\n "
                       "uids: \n  %3\n")
                      .arg(status)
                      .arg(this->JobResponseSets.count())
                      .arg(uids);
  logMsg = QString("Insert job %1. "
                   "Number of jobResponseSet processing: %2.\n "
                   "uids: \n %3\n")
                  .arg(status)
                  .arg(this->JobResponseSets.count())
                  .arg(uids);

  QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
  QString logHeader = currentDateTime + " INFO: ";
  this->Log += logHeader;
  this->Log += logMsg;
  return fullLogMsg;
}

//------------------------------------------------------------------------------
void ctkDICOMInserterJob::setDatabaseFilename(const QString& databaseFilename)
{
  this->DatabaseFilename = databaseFilename;
}

//------------------------------------------------------------------------------
QString ctkDICOMInserterJob::databaseFilename() const
{
  return this->DatabaseFilename;
}

//------------------------------------------------------------------------------
void ctkDICOMInserterJob::setTagsToPrecache(const QStringList& tagsToPrecache)
{
  this->TagsToPrecache = tagsToPrecache;
}

//------------------------------------------------------------------------------
QStringList ctkDICOMInserterJob::tagsToPrecache() const
{
  return this->TagsToPrecache;
}

//------------------------------------------------------------------------------
void ctkDICOMInserterJob::setTagsToExcludeFromStorage(const QStringList& tagsToExcludeFromStorage)
{
  this->TagsToExcludeFromStorage = tagsToExcludeFromStorage;
}

//------------------------------------------------------------------------------
QStringList ctkDICOMInserterJob::tagsToExcludeFromStorage() const
{
  return this->TagsToExcludeFromStorage;
}

//------------------------------------------------------------------------------
ctkAbstractJob* ctkDICOMInserterJob::clone() const
{
  ctkDICOMInserterJob* newInserterJob = new ctkDICOMInserterJob;
  newInserterJob->setDICOMLevel(this->dicomLevel());
  newInserterJob->setPatientID(this->patientID());
  newInserterJob->setStudyInstanceUID(this->studyInstanceUID());
  newInserterJob->setSeriesInstanceUID(this->seriesInstanceUID());
  newInserterJob->setSOPInstanceUID(this->sopInstanceUID());
  newInserterJob->setMaximumNumberOfRetry(this->maximumNumberOfRetry());
  newInserterJob->setRetryDelay(this->retryDelay());
  newInserterJob->setRetryCounter(this->retryCounter());
  newInserterJob->setIsPersistent(this->isPersistent());
  newInserterJob->setMaximumConcurrentJobsPerType(this->maximumConcurrentJobsPerType());
  newInserterJob->setPriority(this->priority());
  newInserterJob->setDatabaseFilename(this->databaseFilename());
  newInserterJob->setTagsToPrecache(this->tagsToPrecache());
  newInserterJob->setTagsToExcludeFromStorage(this->tagsToExcludeFromStorage());

  return newInserterJob;
}

//------------------------------------------------------------------------------
ctkAbstractWorker* ctkDICOMInserterJob::createWorker()
{
  ctkDICOMInserterWorker* worker =
    new ctkDICOMInserterWorker;
  worker->setJob(*this);
  return worker;
}

//------------------------------------------------------------------------------
ctkDICOMJobResponseSet::JobType ctkDICOMInserterJob::getJobType() const
{
  return ctkDICOMJobResponseSet::JobType::Inserter;
}
