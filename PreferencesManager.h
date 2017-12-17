/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__PREFERENCES__MANAGER__H
#define BEEFTEXT__PREFERENCES__MANAGER__H


//**********************************************************************************************************************
/// \brief Preferences manager class
//**********************************************************************************************************************
class PreferencesManager
{
public: // static member functions
   static PreferencesManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
	~PreferencesManager() = default; ///< Default destructor
   void reset(); ///< Reset the preferences to their default values
   QString getInstalledApplicationPath() const; ///< Retrieve the path of the installed application
   void setAlreadyLaunched(); ///< Set the value for the 'First Launch' preference to false
   bool alreadyLaunched() const; ///< Test whether this is the first time the application is launched
   void setFileMarkedForDeletionOnStartup(QString const& path); ///< Get the path of the file marked for deletion on startup
   QString fileMarkedForDeletionOnStartup() const; ///< Set the path of the file marked for deletion on startup
   void clearFileMarkedForDeletionOnStartup(); ///< Reset the file marked for deletion on startup
   void setMainWindowGeometry(QByteArray const& array); ///< Set the geometry of the main window
   QByteArray mainWindowGeometry() const; ///< Get the geometry of the main window
   void setAutoStartAtLogin(bool value); ///< Set the value for the 'Autostart at login' preference
   bool autoStartAtLogin() const; ///< Get the value for the 'Autostart at login' preference
   void setPlaySoundOnCombo(bool value); ///< Set the value for the 'Play sound on combo' preference
   bool playSoundOnCombo() const; ///< Set the value for the 'Play sound on combo' preference
   void setAutoCheckForUpdates(bool value); ///< Set the value for the 'Auto check for updates preference
   bool autoCheckForUpdates() const; ///< Set the value for the 'Auto check for updates preference
   void setUseClipboardForComboSubstitution(bool value); ///< Set the value for the 'Use clipboard for combo substitution' preference
   bool useClipboardForComboSubstitution() const; ///< Get the value for the 'Use clipboard for combo substitution' preference

private: // member functions
   PreferencesManager(); ///< Default constructor
   PreferencesManager(PreferencesManager const&) = delete; ///< Disabled copy constructor
	PreferencesManager(PreferencesManager&&) = delete; ///< Disabled move constructor
	PreferencesManager& operator=(PreferencesManager const&) = delete; ///< Disabled assignment operator
	PreferencesManager& operator=(PreferencesManager&&) = delete; ///< Disabled move assignment operator
   template <typename T> T readSettings(QString const& key, T const& defaultValue = T()) const; ///< Read a value of a given type read from the settings
   
private: // data members
   QSettings settings_; ///< The Qt settings instance
};


#endif // #ifndef BEEFTEXT__PREFERENCES__MANAGER__H